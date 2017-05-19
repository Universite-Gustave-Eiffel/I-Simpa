function vtk_puvw_write ( output_unit, title, node_num, element_num, ...
  element_order, xyz, element_node, p, uvw )

%*****************************************************************************80
%
%% VTK_PUVW_WRITE writes pressure and velocity data to a VTK file.
%
%  Discussion:
%
%    The data is assumed to have been computed by a finite element program
%    for a 3D geometry which has been meshed using tetrahedral elements 
%    of 4 or 10 nodes.
%
%    The solution data includes the pressure and velocity vector at each node.
%
%    Note that the VTK format used here is known as the "legacy" or "old style"
%    format.  It has been superseded by a family of XML based formats.  The
%    appropriate replacement for the VTK format used here is known as "VTU",
%    which is the Visual Toolkit format for unstructured grid data.
%
%    Thanks to Mike Sussman for suggesting that real data should be
%    written with the "double" attribute rather than the "float",
%    JVB, 20 December 2010.
%
%  Licensing:
%
%    This code is distributed under the GNU LGPL license.
%
%  Modified:
%
%    20 December 2010
%
%  Author:
%
%    John Burkardt
%
%  Parameters:
%
%    Input, integer OUTPUT_UNIT, the output unit.
%
%    Input, string TITLE, a title for the data.
%
%    Input, integer NODE_NUM, the number of nodes.
%
%    Input, integer ELEMENT_NUM, the number of elements.
%
%    Input, integer ELEMENT_ORDER, the order of the elements.
%
%    Input, real XYZ(3,NODE_NUM), the node coordinates.
%
%    Input, integer ELEMENT_NODE(ELEMENT_ORDER,ELEMENT_NUM), the
%    nodes that make up each element.  The node indices are zero based.
%
%    Input, real P(1,NODE_NUM), the pressure at each node.
%
%    Input, real UVW(3,NODE_NUM), the velocity at each node.
%
  if ( element_order == 10 )
    fprintf ( 1, '\n' );
    fprintf ( 1, 'VTK_PUVW_WRITE - Note:\n' );
    fprintf ( 1, '  As a temporary measure, we are handling quadratic tets\n' );
    fprintf ( 1, '  as though they were linear.  That is, the element information\n' );
    fprintf ( 1, '  will only use the vertices, not the midside nodes.\n' );
    element_order = 4;
  end

  fprintf ( output_unit, '# vtk DataFile Version 2.0\n' );
  fprintf ( output_unit, '%s\n', title );
  fprintf ( output_unit, 'ASCII\n' );
  fprintf ( output_unit, '\n' );
  fprintf ( output_unit, 'DATASET UNSTRUCTURED_GRID\n' );
  fprintf ( output_unit, 'POINTS %d double\n', node_num );

  for node = 1 : node_num
    fprintf ( output_unit, '  %f  %f  %f\n', xyz(1:3,node) );
  end
%
%  Note that CELL_SIZE uses ELEMENT_ORDER+1.
%
%  Note that the 1-based node indices in ELEMENT_NODE must be
%  converted to 0-based indices before being written out.
%
  cell_size = element_num * ( element_order + 1 );

  fprintf ( output_unit, '\n' );
  fprintf ( output_unit, 'CELLS  %d  %d\n', element_num, cell_size );
  for element = 1 : element_num
    fprintf ( output_unit, '  %d', element_order );
    for order = 1 : element_order
      fprintf ( output_unit, '  %d', element_node(order,element) - 1 );
    end
    fprintf ( output_unit, '\n' );
  end
%
%  VTK has a cell type 24 for quadratic tetrahedrons.  However, we
%  are going to strip the data down to linear tetrahedrons for now,
%  which is cell type 10.
%
  fprintf ( output_unit, '\n' );
  fprintf ( output_unit, 'CELL_TYPES %d\n', element_num );

  if ( element_order == 4 )
    for element = 1 : element_num
      fprintf ( output_unit, '10\n' );
    end
  elseif ( element_order == 10 )
    for element = 1 : element_num
      fprintf ( output_unit, '24\n' );
    end
  end

  fprintf ( output_unit, '\n' );
  fprintf ( output_unit, 'POINT_DATA %d\n', node_num );
  fprintf ( output_unit, 'SCALARS pressure double\n' );
  fprintf ( output_unit, 'LOOKUP_TABLE default\n' );
  for node = 1 : node_num
    fprintf ( output_unit, '  %f\n', p(node) );
  end
  fprintf ( output_unit, 'VECTORS velocity double\n' );
  for node = 1 : node_num
    fprintf ( output_unit, '  %f  %f  %f\n', uvw(1:3,node) );
  end

  return
end
