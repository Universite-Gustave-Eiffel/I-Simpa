#!/usr/bin/env python3
"""Verify the packaged toolbar images and I-Simpa translation catalogs."""
import gettext
import re
import subprocess
import sys
from pathlib import Path

app = Path(sys.argv[1])
resources = app / 'Contents' / 'Resources'
source = Path('src/isimpa/main/i_simpa_main.cpp').read_text()
icons = set(re.findall(r'LoadToolbarBitmap\(resourceFolder, "([^"]+)"\)', source))
if not icons:
    sys.exit('No toolbar icons found in source')
for icon in sorted(icons):
    path = resources / 'Bitmaps' / icon
    if not path.is_file():
        sys.exit(f'Missing toolbar icon: {path}')
    output = subprocess.check_output(['sips', '-g', 'format', '-g', 'pixelWidth', '-g', 'pixelHeight', str(path)], text=True)
    if not re.search(r'format:\s*png', output) or not re.search(r'pixelWidth:\s*[1-9]\d*', output) or not re.search(r'pixelHeight:\s*[1-9]\d*', output):
        sys.exit(f'Invalid toolbar PNG: {path}\n{output}')
for po in sorted(Path('src/isimpa/lang').glob('*.po')):
    catalog = resources / 'share' / 'locale' / po.stem / 'LC_MESSAGES' / 'isimpa.mo'
    if not catalog.is_file():
        sys.exit(f'Missing translation catalog: {catalog}')
    with catalog.open('rb') as f:
        gettext.GNUTranslations(f)
    print(f'Valid catalog: {catalog}')
print(f'Validated {len(icons)} toolbar images')
