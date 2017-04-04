function [p,f,t] = read_tetgenmesh( file )

% READ_TETGENMESH  Reads Tetgen meshes
%   Reads the native Tetgen format mesh-files into Matlab. The output
%   [p,f,t] (nodes,faces,tetras) is similar to the output of init/poimesh
%   of the PDEtool toolbox, which is [p,e,t] (nodes,edges,triangles).
%   This function loads only the simplest tetrahedral Tetgen meshes
%   (no volume/facet holes or any fance attributes).
%
%   Tetgen manual: http://tetgen.berlios.de/files/tetgen-manual.pdf
%   (13.07.2012)
%
%   Let P,F,T be the number of nodes, faces, and tetras, respectively.
%   
%   Nodes (p):
%   This function is designed to read the .node files which do not have
%   attributes (see Tetgen documentation 4.1.1. on page 31), but have
%   boundary markers. p is a [4,P] matrix, where 3 first rows are the
%   coordinates, and the last one the boundary marker (1=boundary node,
%   0=inner node)
%
%   Faces (f):
%   This function is designed to read .face files which do not specify
%   holes (see Tetgen documentation 4.1.2 on page 33), but have boundary
%   markers. f is a [4,F] matrix, where 3 first rows are the node indices
%   (for node matrix p), and the last one the boundary marker (1=boundary
%   face, 0=inner face). A simply connected, no hole domain which has not
%   been divided into parts has only boundary faces?
%
%   Tetras (t):
%   This function is designed to read .ele files which do not specify
%   regional attributes (see Tetgen documentation 4.1.5 on page 38).
%   t is a [4,T] matrix where the 4 rows are the node indices (for node
%   matrix p).
%
%
% SYNTAX:   [p,f,t] = readmesh( file )
%
% IN:   file    a collection of mesh files created by Tetgen
%               file.node/face/ele
%                          
% OUT:  p       points by coordinates
%       f       boundary faces by points
%       t       tetras by points
%

%
%   Author:  Immanuel Anjam, University of Jyvaskyla
%   Contact: immanuel.anjam@gmail.com
%   Date:    13.07.2012
%   Version: 1.1
%


% nodes for p

fid=fopen([file,'.node'], 'rt');         %open the file
title = fgetl(fid);                      %read in the header
title = str2num(title);
cols  = title(1);
rows  = title(2) + title(4) + 1;
data  = fscanf(fid, '%e',[rows,cols]);   %read in data
p     = data(2:rows,1:cols);             %ignore the first row
fclose(fid);                             %close the file


% boundary faces for f

fid=fopen([file,'.face'], 'rt');         %open the file
title = fgetl(fid);                      %read in the header
title = str2num(title);
cols  = title(1);
rows  = title(2) + 4;
data  = fscanf(fid, '%i',[rows,cols]);   %read in data
f     = data(2:rows,1:cols);             %ignore the first row
fclose(fid);                             %close the file


% elements for t

fid=fopen([file,'.ele'], 'rt');          %open the file
title = fgetl(fid);                      %read in the header
title = str2num(title);
cols  = title(1);
rows  = title(2) + title(3) + 1;
data  = fscanf(fid, '%i',[rows,cols]);   %read in data
t     = data(2:rows,1:cols);             %ignore the first row
fclose(fid);                             %close the file


% boundary edges for e
% these are the corner edges of a domain, not so useful
%{
fid=fopen([file,'.edge'], 'rt');         %open the file
title = fgetl(fid);                      %read in the header
title = str2num(title);
cols  = title(1);
data  = fscanf(fid, '%i',[3,cols]);      %read in data
e     = data(2:3,1:cols);                %ignore the first row
fclose(fid);                             %close the file
%}