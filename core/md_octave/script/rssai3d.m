% Exemple de la documentation dela fonction isocolor
[X Y Z] = meshgrid(1:20,1:20,1:20);
T = sqrt(X.^2 + Y.^2 + Z.^2);
 
% Parcours curviligne 
r = 17;
th = linspace(pi/2,pi,50);
xx = 20+r*cos(th);
yy = 1+r*sin(th);
 
% Définition de la grille de point
xx = repmat(xx,size(T,3),1);
yy = repmat(yy,size(T,3),1);
zz = repmat(squeeze(Z(1,1,:)),1,size(xx,2));
 
% Sommets du maillage
FV.Vertices = [xx(:) yy(:) zz(:)];
 
% Faces du maillage
nz = size(Z,1);
nn = numel(zz)/nz;
FV.Faces = [];
t = [1:nz-1 ; 2:nz ; nz+2:2*nz ; nz+1: 2*nz-1].';
for n = 1:nn-1
    FV.Faces = [FV.Faces ; t+nz*(n-1)];
end
 
% Interpolation des données pour les couleurs
FV.FaceVertexCData  = interp3(X,Y,Z,T,xx(:),yy(:),zz(:));
FV.FaceColor = 'flat';
 
% Affichage du résultat
figure
slice(X,Y,Z,T,[],[],[5 10 15])
hold on
patch(FV);
 
axis equal vis3d
view(3)
xlabel('x')
ylabel('y')
zlabel('z')
 
%set(gca,'xlim',[1 20],'ylim',[1 20],'zlim',[1 20])