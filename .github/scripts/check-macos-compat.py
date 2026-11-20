#!/usr/bin/env python3
"""Check x86_64 Mach-O deployment targets and non-system dependencies."""
import os
import re
import subprocess
import sys
from pathlib import Path

TARGET = (14, 0)
MAGIC = {bytes.fromhex(x) for x in ('feedface', 'cefaedfe', 'feedfacf', 'cffaedfe', 'cafebabe', 'bebafeca', 'cafebabf', 'bfbafeca')}
seen = set()
errors = []
queue = []
executable_dir = None

def run(*args):
    return subprocess.check_output(args, text=True, stderr=subprocess.STDOUT)

def add(path):
    path = Path(path)
    if not path.exists():
        errors.append(f'Missing dependency: {path}')
    else:
        queue.append(path.resolve())

def scan(root):
    root = Path(root)
    if root.is_file():
        add(root)
        return
    for parent, _, files in os.walk(root):
        for name in files:
            path = Path(parent) / name
            try:
                with path.open('rb') as f:
                    if f.read(4) in MAGIC:
                        add(path)
            except OSError as exc:
                errors.append(f'Cannot read {path}: {exc}')

for root in sys.argv[1:]:
    scan(root)
if not queue:
    sys.exit('No Mach-O files found')

while queue:
    path = queue.pop()
    if path in seen:
        continue
    seen.add(path)
    try:
        run('lipo', str(path), '-verify_arch', 'x86_64')
        loads = run('otool', '-arch', 'x86_64', '-l', str(path))
        version = re.search(r'cmd LC_BUILD_VERSION\s+cmdsize \d+\s+platform (?:1|MACOS)\s+minos ([\d.]+)', loads)
        if not version:
            version = re.search(r'cmd LC_VERSION_MIN_MACOSX\s+cmdsize \d+\s+version ([\d.]+)', loads)
        if not version:
            raise ValueError('no macOS deployment target found')
        minimum = version.group(1)
        parts = tuple(int(x) for x in minimum.split('.'))
        if parts > TARGET + (0,) * max(0, len(parts) - len(TARGET)):
            raise ValueError(f'minimum macOS {minimum} exceeds 14.0')
        print(f'{path}: x86_64, macOS {minimum}', flush=True)
        rpaths = re.findall(r'cmd LC_RPATH\s+cmdsize \d+\s+path (.+?) \(offset \d+\)', loads)
        own_id = re.search(r'cmd LC_ID_DYLIB\s+cmdsize \d+\s+name (.+?) \(offset \d+\)', loads)
        links = run('otool', '-arch', 'x86_64', '-L', str(path)).splitlines()[1:]
        for line in links:
            match = re.match(r'\s*(.*?) \(compatibility version ', line)
            if not match:
                continue
            dep = match.group(1)
            if own_id and dep == own_id.group(1):
                continue
            if dep == '/usr/local/opt/python@3.14/Frameworks/Python.framework/Versions/3.14/Python' and os.environ.get('SONOMA_PYTHON_LIBRARY'):
                add(os.environ['SONOMA_PYTHON_LIBRARY'])
                continue
            if dep.startswith(('/usr/lib/', '/System/Library/')):
                continue
            def expand(value):
                return value.replace('@loader_path', str(path.parent)).replace('@executable_path', str(executable_dir or path.parent))
            if dep.startswith('@rpath/'):
                candidates = [Path(expand(r)) / dep[len('@rpath/'):] for r in rpaths]
                found = next((p for p in candidates if p.exists()), None)
                if not found:
                    raise ValueError(f'unresolved {dep}; rpaths={rpaths}')
                add(found)
            elif dep.startswith('@'):
                resolved = expand(dep)
                if resolved.startswith('@'):
                    raise ValueError(f'unresolved dependency {dep}')
                add(resolved)
            elif dep.startswith('/'):
                add(dep)
            else:
                raise ValueError(f'unresolved relative dependency {dep}')
    except (subprocess.CalledProcessError, OSError, ValueError) as exc:
        errors.append(f'{path}: {exc}')

print(f'Inspected {len(seen)} Mach-O files', flush=True)
if errors:
    print('\n'.join(errors), file=sys.stderr)
    sys.exit(1)
