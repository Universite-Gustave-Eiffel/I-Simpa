function threed_to_vtk ( x, e_conn, uvwp, output_filename, title )

%*****************************************************************************80
%
%% THREED_TO_VTK writes out a 3D FEM flow solution to a VTK file.
%
%  Discussion:
%
%    Note that the VTK format used here is known as the "legacy" or "old style"
%    format.  It has been superseded by a family of XML based formats.  The
%    appropriate replacement for the VTK format used here is known as "VTU",
%    which is the Visual Toolkit format for unstructured grid data.
%
%  Licensing:
%
%    This code is distributed under the GNU LGPL license.
%
%  Modified:
%
%    25 March 2009
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
%    form each element.  Node indices are 1-based.
%
%    Input, real UVWP(NODE_NUM,4), U, V, and W velocity components and 
%    pressure at each node.
%
%    Input, string OUTPUT_FILENAME, the name of the output file.
%    By convention, this file should have the extension ".vtk".
%
%    Input, string TITLE, a title for the data.
%
  [ node_num, dim_num ] = size ( x );
  [ element_num,  element_order ] = size ( e_conn );
%
%  Open the output file.
%
  if ( isempty ( output_filename ) )
    output_filename = 'ns3d_fem.vtk';
  end

  output_unit = fopen ( output_filename, 'w' );
%
%  Transpose or otherwise modify the data.
%
  xyz = zeros ( 3, node_num );
  xyz(1:3,1:node_num) = x(1:node_num,1:3)';

  element_node = zeros ( element_order, element_num );
  element_node(1:element_order,1:element_num) = ...
    e_conn(1:element_num,1:element_order)' - 1;

  p = zeros ( 1, node_num );
  p(1,1:node_num) = uvwp(1:node_num,3)';

  uvw = zeros ( 3, node_num );
  uvw(1:3,1:node_num) = uvwp(1:node_num,1:3)';
%
%  Write the data.
%
  vtk_puvw_write ( output_unit, title, node_num, element_num, ...
    element_order, xyz, element_node, p, uvw );

  fclose ( output_unit ) ;

  fprintf ( 1, '\n' );
  fprintf ( 1, 'THREED_TO_VTK\n' );
  fprintf ( 1, '  The data was written to "%s"\n', output_filename );

  return
end
