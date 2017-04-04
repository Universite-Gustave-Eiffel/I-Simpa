%{
 %% Function to create an export to paraview file "vtk" format

 sx: Sigma X stresses
 sy: Sigma Y stresses
 sz: Sigma Z stresses
 ex: Deformations X axis
 ey: Deformations Y axis
 ez: Deformations Z axis
 xnod: XNod matrix
 LaG: Local to Global matrix
 nno: Number of nodes
 elemType: Shape of the finite element. Ex: Quadrilateral, Triangle, Tetrahedra
 ndim: Number of dimensions 2 or 3.
 filename: Filename of the vtk file.
%}
function export_vers_paraview(sx, sy, sz, ex, ey, ez, xnod, LaG, nno, elemType, ndim, filename)
    
    %% Initial parameters
    nef = size(LaG,1);
    totalNno = size(xnod,1);  % Total of nodes 
    cell_id = 1;  % Cell id indicate the id of the type of finite element
    
    %% ***** ***** ***** Writing the vtk file ***** ***** *****
    fid = fopen([filename '.vtk'],'w');
    
    %% Writing the header from the vtk file
    fprintf(fid,'# vtk DataFile Version 3.1 \n');
    fprintf(fid,'This file was created by matlab source code \n');
    fprintf(fid,'ASCII \n');
    fprintf(fid,'DATASET UNSTRUCTURED_GRID \n');
    %fprintf(fid,'MESH dimension %3.0f   Elemtype %s   Nnode %2.0f \n \n', ndim, eletyp, nno_por_elem);
    fprintf(fid,'POINTS %3.0f  FLOAT \n', totalNno);
    
    %% The cell_id depending of the shape of the finite element
   
                cell_id = '10';
                
                % This is to compatibilize the hexahedra from GID with the
                % hexahedra from VTK standar
                LaG(:,[1 2 3 4 5 6 7 8 9 10 11 12 17 18 19 20 13 14 15 16]) = LaG; 
            end
    end
    
    %% Writing the coordinates of each node
    
    for i=1:totalNno        
        fprintf(fid, [repmat('%12.5f ',1,ndim) '\n'], xnod(i,:));
    end
    
    %% Writing the cells or nodes   
    totalCells = nef*(nno+1);
    LaG_ = LaG - 1;
    fprintf(fid, '\n');
    fprintf(fid,'CELLS %3.0f %3.0f \n', nef, totalCells);
    for i=1:nef        
        fprintf(fid, ['%5d ' repmat('%5d ',1,nno) '\n'], nno, LaG_(i,:));
    end
    
    %% Cell types define the type of finite element 
    % For example Quedrilateral of 8 nodes is equal to 23
    fprintf(fid, '\n');
    fprintf(fid,'CELL_TYPES %5d \n', nef);
    
    cells = '';
    for i=1:nef        
        cells = [cells cell_id ' '];        
    end
    fprintf(fid, '%s', cells);
   
    %% Writing the header of the stresses and deformations    
    nef = size(sx,1);
    if sx ~= 0    
        fprintf(fid, '\n');    
        fprintf(fid, '\n'); 
        fprintf(fid,'POINT_DATA %3.0f \n', nef);
    
        %% Writing the stresses sx

        fprintf(fid,'SCALARS Sigma_X float \n');
        fprintf(fid,'LOOKUP_TABLE default \n');
        for i=1:nef
            fprintf(fid,'%12.5g \n', sx(i));
        end
    end
    
    if sy ~= 0
        %% Writing the stresses sy    
        fprintf(fid, '\n');
        fprintf(fid,'SCALARS Sigma_Y float \n');
        fprintf(fid,'LOOKUP_TABLE default \n');
        for i=1:nef
            fprintf(fid,'%12.5g \n', sy(i));
        end
    end
    
    if ex ~= 0
        %% Writing the deformation Ex
        fprintf(fid, '\n');        
        fprintf(fid,'SCALARS Ex float \n');
        fprintf(fid,'LOOKUP_TABLE default \n');
        for i=1:nef
            fprintf(fid,'%12.5g \n', ex(i));
        end
    end
    
    if ey ~= 0
        %% Writing the deformation Ex
        fprintf(fid, '\n');        
        fprintf(fid,'SCALARS Ey float \n');
        fprintf(fid,'LOOKUP_TABLE default \n');
        for i=1:nef
            fprintf(fid,'%12.5g \n', ey(i));
        end
    end
    
    fclose(fid); 
return;