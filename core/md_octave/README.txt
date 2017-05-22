Input files for Octave computation core

scene_sources.txt
Source position and power for each third octave band
posX, posY, posZ, 50, 63,80,100,125,160,200,250,315,400,500,630,800,1000,1250,1600,2000,3150,4000,5000,6300,8000,10000,12500,16000,20000


scene_nodes.txt
Vertices of the tetrahedral mesh
x y z

scene_elements.txt
Vertices index (1-n) of tetrahedral elements and the volume identifier.
A, B, C, D, idVolume

scene_faces.txt
Material identifier of each faces
A, B, C, materialId

scene_materials_absorption.txt
Material Identifier and absorption value(0-1) for each third octave band
materialId, 50, 63,80,100,125,160,200,250,315,400,500,630,800,1000,1250,1600,2000,3150,4000,5000,6300,8000,10000,12500,16000,20000

scene_materials_transmission.txt
Material Identifier and transmission attenuation(dB) for each third octave band
materialId, 50, 63,80,100,125,160,200,250,315,400,500,630,800,1000,1250,1600,2000,3150,4000,5000,6300,8000,10000,12500,16000,20000

output

scene_WStatioFields.txt

Puissance sur chaque sommet de 100Hz - 5 kHz