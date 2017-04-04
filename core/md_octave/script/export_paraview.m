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
function export_paraview(x, xnod,   nno, elemType, ndim, filename)
( x, e_conn, p, filename, title )

 
%  Parameters:
%
%    Input, real X(NODE_NUM,3), the node coordinates.
%
%    Input, integer E_CONN(ELEMENT_NUM,ELEMENT_ORDER), the nodes that
%    form each element.
%
%    Input, real UVWP(NODE_NUM,4), U, V, and W velocity components and 
%    pressure at each node.
%
%    Input, string OUTPUT_FILENAME, the name of the output file.
%    By convention, this file should have the extension ".vtu".
%
%    Input, string TITLE, a title for the data.
%
  [ totalNno, dim_num ] = size ( x );
  [ nef,  element_order ] = size ( e_conn );
%
%  Open the output file and write the data to it.
%
  

    
    %% Initial parameters
  
    
    %% ***** ***** ***** Writing the vtk file ***** ***** *****
    fid = fopen([filename '.vtk'],'w');
    
    %% Writing the header from the vtk file
    fprintf(fid,'# vtk DataFile Version 3.1 \n');
    fprintf(fid,'PRAX matlab source code \n');
    fprintf(fid,'ASCII \n');
    fprintf(fid,'DATASET UNSTRUCTURED_GRID \n');
    %fprintf(fid,'MESH dimension %3.0f   Elemtype %s   Nnode %2.0f \n \n', ndim, eletyp, nno_por_elem);
    fprintf(fid,'POINTS %3.0f  FLOAT \n', totalNno);
    
    %% The cell_id depending of the shape of the finite element
 
                cell_id = '10';% Cell id indicate the id of the type of finite element
  
    
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
    
    fclose(fid); 
return;