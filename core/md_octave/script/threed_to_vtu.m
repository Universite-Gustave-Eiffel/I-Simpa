function threed_to_vtu ( x, e_conn, uvwp, output_filename, title )

%% THREED_TO_VTU writes out a 3D FEM flow solution to a VTU file.
%
%  Discussion:
%
%    The VTU file can be read and displayed by the Paraview program.
%
%  Licensing:
%
%    This code is distributed under the GNU LGPL license.
%
%  Modified:
%
%    13 April 2009
%
%  Author:
%
%    John Burkardt
%
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
  [ node_num, dim_num ] = size ( x );
  [ element_num,  element_order ] = size ( e_conn );
%
%  Open the output file and write the data to it.
%
  if ( isempty ( output_filename ) )
    output_filename = 'ns3d_fem.vtu';
  end

  output_unit = fopen ( output_filename, 'w' );

  vtu_puvw_write ( output_unit, title, node_num, element_num, ...
    element_order, x', e_conn', uvwp(:,4), uvwp(:,1:3)' );

  fclose ( output_unit ) ;

  fprintf ( 1, '\n' );
  fprintf ( 1, 'THREED_TO_VTU\n' );
  fprintf ( 1, '  Solution data written to "%s"\n', output_filename );

  return
end