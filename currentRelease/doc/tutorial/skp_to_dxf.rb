# SketchUp to DXF STL Converter
# Last edited: April 12, 2010
# Authors: Nathan Bromham, Konrad Shroeder

require 'sketchup.rb'

def dxf_export_mesh_file
      model = Sketchup.active_model
      model_filename = File.basename(model.path)
      if( model_filename == "" )
        model_filename = "model"
      end
      ss = model.selection
      $stl_conv = 1.0
      $group_count = 0
      $component_count = 0
      $face_count = 0
      $line_count = 0
      entities = model.entities
      if (Sketchup.version_number==7)
        model.start_operation("export_dxf_mesh",true)
      else
         model.start_operation("export_dxf_mesh")
      end
      if ss.empty?
         answer = UI.messagebox("No objects selected. Export entire model?", MB_YESNOCANCEL)
         if( answer == 6 )
            export_ents = model.entities
         else
            export_ents = ss
         end
      else
         export_ents = Sketchup.active_model.selection
      end
      if (export_ents.length > 0)
         #get units for export
         dxf_dxf_units_dialog
         #get DXF export option
         dxf_option = dxf_dxf_options_dialog
         if (dxf_option =="stl")
          file_type="stl"
         else
          file_type="dxf"
         end
         #exported file name
         out_name = UI.savepanel( file_type+" file location", "." , "#{File.basename(model.path).split(".")[0]}." +file_type )
         $mesh_file = File.new( out_name , "w" )  
         model_name = model_filename.split(".")[0]
         dxf_header(dxf_option,model_name)
         # Recursively export faces and edges, exploding groups as we go.
         #Count "other" objects we can't parse.
         others = dxf_find_faces(0, export_ents, Geom::Transformation.new(), model.active_layer.name,dxf_option)
         dxf_end(dxf_option,model_name)
         UI.messagebox( $face_count.to_s + " faces exported " + $line_count.to_s + " lines exported\n" + others.to_s + " objects ignored" )
      end
      model.commit_operation
end

def dxf_find_faces(others, entities, tform, layername,dxf_option)
   entities.each do |entity|
      #Face entity
      if( entity.typename == "Face")
       case dxf_option
       when "polylines"
        dxf_write_polyline(entity,tform,layername)
       when "polyface mesh"
        dxf_write_polyface(entity,tform,layername)
       when "triangular mesh"
         dxf_write_face(entity,tform,layername)
       when "stl"
         dxf_write_stl(entity,tform)     
       end
     #Edge entity
      elsif( entity.typename == "Edge") and((dxf_option=="lines")or(entity.faces.length==0 and dxf_option!="stl"))
       dxf_write_edge(entity, tform, layername)
     #Group entity
      elsif( entity.typename == "Group")
         if entity.name==""
           entity.name="GROUP"+$group_count.to_s
           $group_count+=1
         end
         others = dxf_find_faces(others, entity.entities, tform * entity.transformation, entity.name,dxf_option)
      #Componentinstance entity
      elsif( entity.typename == "ComponentInstance")
         if entity.name==""
           entity.name="COMPONENT"+$component_count.to_s
           $component_count+=1
         end
         others = dxf_find_faces(others, entity.definition.entities, tform * entity.transformation, entity.name,dxf_option)
      else
         others = others + 1
      end
   end
   others
end

def dxf_transform_edge(edge, tform)
   points=[]
   points.push(dxf_transform_vertex(edge.start, tform))
   points.push(dxf_transform_vertex(edge.end, tform))
   points
end

def dxf_transform_vertex(vertex, tform)
   point = Geom::Point3d.new(vertex.position.x, vertex.position.y, vertex.position.z)
   point.transform! tform
   point
end

def dxf_write_edge(edge, tform, layername)
  points = dxf_transform_edge(edge, tform)
  $mesh_file.puts( "  0\nLINE\n 8\n"+layername+"\n")
  for j in 0..1 do
    $mesh_file.puts((10+j).to_s+"\n"+(points[j].x.to_f * $stl_conv).to_s)#x
    $mesh_file.puts((20+j).to_s+"\n"+(points[j].y.to_f * $stl_conv).to_s)#y
    $mesh_file.puts((30+j).to_s+"\n"+(points[j].z.to_f * $stl_conv).to_s)#z
  end
  $line_count+=1
end

def dxf_write_polyline(face, tform,layername)
 face.loops.each do |aloop|
  $mesh_file.puts("  0\nPOLYLINE\n 8\n"+layername+"\n 66\n     1")
  $mesh_file.puts("70\n    8\n 10\n0.0\n 20\n 0.0\n 30\n0.0")
  for j in 0..aloop.vertices.length do
    if (j==aloop.vertices.length)
      count = 0
    else
      count = j
    end
    point = dxf_transform_vertex(aloop.vertices[count],tform)
    $mesh_file.puts( "  0\nVERTEX\n  8\nMY3DLAYER")
    $mesh_file.puts("10\n"+(point.x.to_f * $stl_conv).to_s)
    $mesh_file.puts("20\n"+(point.y.to_f * $stl_conv).to_s)
    $mesh_file.puts("30\n"+(point.z.to_f * $stl_conv).to_s)
    $mesh_file.puts( " 70\n     32")
  end
  if (aloop.vertices.length > 0)
    $mesh_file.puts( "  0\nSEQEND")
  end
 end
 $face_count+=1
end


def dxf_write_face(face,tform, layername)
  mesh = face.mesh 0
  mesh.transform! tform
  polygons = mesh.polygons
  polygons.each do |polygon|
  if (polygon.length > 2)
    flags = 0
    $mesh_file.puts( "  0\n3DFACE\n 8\n"+layername)
    for j in 0..polygon.length do
      if (j==polygon.length)
        count = polygon.length-1
      else
        count = j
      end
      #check edge visibility
      if ((polygon[count]<0))
       flags+=2**j
      end
      $mesh_file.puts((10+j).to_s+"\n"+(mesh.point_at(polygon[count].abs).x.to_f * $stl_conv).to_s)
      $mesh_file.puts((20+j).to_s+"\n"+(mesh.point_at(polygon[count].abs).y.to_f * $stl_conv).to_s)
      $mesh_file.puts((30+j).to_s+"\n"+(mesh.point_at(polygon[count].abs).z.to_f * $stl_conv).to_s)
    end
    #edge visibiliy flags
    $mesh_file.puts("70\n"+flags.to_s)  
    end
  end
  $face_count+=1
end

def dxf_write_stl(face,tform)
  mesh = face.mesh 7
  mesh.transform! tform
  polygons = mesh.polygons
  polygons.each do |polygon|
  if (polygon.length == 3)
      $mesh_file.puts( "facet normal " + mesh.normal_at(polygon[0].abs).x.to_s + " " + mesh.normal_at(polygon[0].abs).y.to_s + " " + mesh.normal_at(polygon[0].abs).z.to_s)
      $mesh_file.puts( "outer loop")
      for j in 0..2 do
         $mesh_file.puts("vertex " + (mesh.point_at(polygon[j].abs).x.to_f * $stl_conv).to_s + " " + (mesh.point_at(polygon[j].abs).y.to_f * $stl_conv).to_s + " " + (mesh.point_at(polygon[j].abs).z.to_f * $stl_conv).to_s)
      end
      $mesh_file.puts( "endloop\nendfacet")
    end
  end
  $face_count+=1
end

def dxf_write_polyface(face,tform,layername)
  mesh = face.mesh 0
  mesh.transform! tform
  polygons = mesh.polygons
  points = mesh.points
  $mesh_file.puts("  0\nPOLYLINE\n 8\n"+layername+"\n 66\n     1")
  $mesh_file.puts("10\n0.0\n 20\n 0.0\n 30\n0.0\n")
  $mesh_file.puts("70\n    64\n") #flag for 3D polyface
  $mesh_file.puts("71\n"+mesh.count_points.to_s)
  $mesh_file.puts("72\n   1")
  #points
  points.each do |point| 
      $mesh_file.puts( "  0\nVERTEX\n  8\n"+layername)
      $mesh_file.puts("10\n"+(point.x.to_f * $stl_conv).to_s)
      $mesh_file.puts("20\n"+(point.y.to_f * $stl_conv).to_s)
      $mesh_file.puts("30\n"+(point.z.to_f * $stl_conv).to_s)
      $mesh_file.puts( " 70\n     192")
  end
  #polygons
  polygons.each do |polygon| 
      $mesh_file.puts( "  0\nVERTEX\n  8\n"+layername)
      $mesh_file.puts("10\n0.0\n 20\n 0.0\n 30\n0.0\n")
      $mesh_file.puts( " 70\n     128")
      $mesh_file.puts( " 71\n"+polygon[0].to_s)
      $mesh_file.puts( " 72\n"+polygon[1].to_s)
      $mesh_file.puts( " 73\n"+polygon[2].to_s)
      if (polygon.length==4)
        $mesh_file.puts( " 74\n"+polygon[3]..abs.to_s)
      end
  end
  $mesh_file.puts( "  0\nSEQEND")
  $face_count+=1
end

def dxf_dxf_options_dialog
   options_list=["polyface mesh","polylines","triangular mesh","lines","stl"].join("|")
   prompts=["Export to DXF options"]
   enums=[options_list]
   values=["polyface mesh"]
   results = inputbox prompts, values, enums, "Choose which entities to export"
   return if not results
   results[0]
end

def dxf_dxf_units_dialog
   cu=Sketchup.active_model.options[0]["LengthUnit"]
   case cu
   when 4
      current_unit= "Meters"
   when 3
      current_unit= "Centimeters"
   when 2
      current_unit= "Millimeters"
   when 1
      current_unit= "Feet"
   when 0
      current_unit= "Inches"
   end
   units_list=["Meters","Centimeters","Millimeters","Inches","Feet"].join("|")
   prompts=["Export unit: "]
   enums=[units_list]
   values=[current_unit]
   results = inputbox prompts, values, enums, "Export units"
   return if not results
   case results[0]
   when "Meters"
      $stl_conv=0.0254
   when "Centimeters"
      $stl_conv=2.54
   when "Millimeters"
      $stl_conv=25.4
   when "Feet"
      $stl_conv=0.0833333333333333
   when "Inches"
      $stl_conv=1
   end
end

def dxf_header(dxf_option,model_name)
  if (dxf_option=="stl")
   $mesh_file.puts( "solid " + model_name)
  else
   $mesh_file.puts( " 0\nSECTION\n 2\nENTITIES")
  end
end

def dxf_end(dxf_option,model_name)
  if (dxf_option=="stl")
    $mesh_file.puts( "endsolid " + model_name)
  else
    $mesh_file.puts( " 0\nENDSEC\n 0\nEOF")
  end
  $mesh_file.close
end

if( not file_loaded?("skp_to_dxf.rb") )
   add_separator_to_menu("Tools")
   UI.menu("Tools").add_item("Export to DXF or STL") { dxf_export_mesh_file }
end

file_loaded("skp_to_dxf.rb")
