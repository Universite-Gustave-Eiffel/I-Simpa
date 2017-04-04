p = patch(isosurface(x,y,z,w,2))
isonormals(x,y,z,LpdB,p)
%set(p,'FaceColor','blue','EdgeColor','none');
daspect([1,1,1])
view(3); axis tight