function [] = sauv_tecplot(x,e_conn,u,filename,title)

%*****************************************************************************80
%
%  threed_to_tecplot - writes out a 3D finite element solution to a format readable by Tecplot.
%
%  Usage:
%         [] = threed_to_tecplot(x,e_conn,u,filename,title)
%
%  Variables:     x
%                      spatial coordinates  (nodes x 3)
%                 e_conn
%                      element connectivity matrix  (elements x (either 4 or 10))
%                 u
%                      field variables
%                 filename
%                      name for the output tecplot file
%                 title
%                      title string required by tecplot
%------------------------------------------------------------------------------

[n_node,n_var] = size(u);
[n_elem,n_dof] = size(e_conn);

if (n_dof==4)
  [fid] = fopen(filename,'w');

  fprintf(fid,strcat(strcat('Title="',title),'"\n'));

  temp_str = 'Variables="x","y","z"';
  for i=1:n_var
    temp_str = strcat(strcat(temp_str,strcat(strcat(',"',title),strcat(num2str(i),'"'))));
  end
  temp_str = strcat(temp_str,'\n');
  fprintf(fid,temp_str);

  temp_str = 'Zone N = ';
  temp_str = strcat( temp_str, num2str(n_node) );
  temp_str = strcat( temp_str, ', E = ' );
  temp_str = strcat( temp_str, num2str(n_elem) );
  temp_str = strcat( temp_str, ', DATAPACKING = POINT, ZONETYPE = FETETRAHEDRON\n' );
  fprintf(fid,temp_str);

  print_str = ' %10.5g  %10.5g  %10.5g';
  for i=1:n_var
    print_str = strcat( print_str, ' %10.5g ' );
  end
  print_str = strcat( print_str, '\n' );

  for i=1:n_node
    fprintf(fid,print_str,x(i,:),u(i,:));
  end

  print_str = ' %6d  %6d  %6d   %6d \n';
  for i=1:n_elem
    fprintf(fid,print_str,e_conn(i,1:4));  %  coarse version
  end

  fclose(fid);
  % end function output_to_tecplot
end



if (n_dof==10)
  [fid] = fopen(filename,'w');

  fprintf(fid,strcat(strcat('Title="',title),'"\n'));

  temp_str = 'Variables="x","y","z"';
  for i=1:n_var
    temp_str = strcat(strcat(temp_str,strcat(',"vec',strcat(num2str(i),'"'))));
  end
  temp_str = strcat(temp_str,'\n');
  fprintf(fid,temp_str);

  temp_str = 'Zone N = ';
  temp_str = strcat( temp_str, num2str(n_node) );
  temp_str = strcat( temp_str, ', E = ' );
  temp_str = strcat( temp_str, num2str(7*n_elem) );
  temp_str = strcat( temp_str, ', DATAPACKING = POINT, ZONETYPE = FETETRAHEDRON\n' );
  fprintf(fid,temp_str);

  print_str = ' %10.5g  %10.5g  %10.5g';
  for i=1:n_var
    print_str = strcat( print_str, ' %10.5g ' );
  end
  print_str = strcat( print_str, '\n' );

  for i=1:n_node
    fprintf(fid,print_str,x(i,:),u(i,:));
  end

  print_str = ' %6d  %6d  %6d   %6d \n';
  for i=1:n_elem
  %  fprintf(fid,print_str,e_conn(i,1:4));  %  coarse version
    fprintf(fid,print_str,[e_conn(i,5)  e_conn(i,2)  e_conn(i,6)  e_conn(i,8)]);
    fprintf(fid,print_str,[e_conn(i,7)  e_conn(i,6)  e_conn(i,3)  e_conn(i,9)]);
    fprintf(fid,print_str,[e_conn(i,10) e_conn(i,8)  e_conn(i,9)  e_conn(i,4)]);
    fprintf(fid,print_str,[e_conn(i,5)  e_conn(i,6)  e_conn(i,1)  e_conn(i,8)]);
    fprintf(fid,print_str,[e_conn(i,7)  e_conn(i,1)  e_conn(i,6)  e_conn(i,9)]);
    fprintf(fid,print_str,[e_conn(i,10) e_conn(i,9)  e_conn(i,8)  e_conn(i,1)]);
    fprintf(fid,print_str,[e_conn(i,6)  e_conn(i,1)  e_conn(i,8)  e_conn(i,9)]);
  end

  fclose(fid);
  % end function output_to_tecplot
end
