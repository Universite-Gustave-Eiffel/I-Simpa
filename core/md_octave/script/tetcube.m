dimX=10;
dimY=1.5;
dimZ=2.4;
nx=50;
ny=8;
nz=12;

[T,V] = regular_tetrahedral_mesh(nx,ny,nz,dimX,dimY,dimZ);
tetramesh(T,V); 