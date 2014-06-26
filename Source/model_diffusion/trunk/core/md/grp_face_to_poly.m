function obj = grp_face_to_poly(VERTS,TRI)
% poly3 Retourne une surface polygone dans le le modèle en 3 dimensions
% 
% TRI doit être une matrice d'indice des sommets des triangles coplanaires
% Exemple :
% geomplot(grp_face_to_poly([0,0,0;0,2,0;2,2,0;2,0,0],[1,2,4;2,3,4]));
if size(VERTS,2)~=3
    error('The number of column must be 3.');
end
if size(VERTS,1)<3
    error('The number of lines must be greater than 2.');
end
if size(TRI,1)==1
    %disp('Warning, a polygon is a triangle and the model may not be able to be meshed, please inspect face/material association !');
    %TRI
    obj = face3(VERTS(TRI(1,1:3),1),VERTS(TRI(1,1:3),2),VERTS(TRI(1,1:3),3));
    return;
end

%3 Sommets de base du premier triangle
A=VERTS(TRI(1,1),1:3);
B=VERTS(TRI(1,2),1:3);
C=VERTS(TRI(1,3),1:3);

%Vecteurs cotés
AB=B-A;
AC=C-A;

%Calcul de la matrice de transformation
I=AB/norm(AB);
K=cross(AB,AC)/norm(cross(AB,AC));
J=cross(K,I);
M=[I;J;K];


%Conversion des sommets selon les coordonnées locales 2D
LOCVERTS=M*[VERTS(:,1)-A(1) VERTS(:,2)-A(2) VERTS(:,3)-A(3)]';

%Pour chaque triangle
%Création des triangles de la figure
tri_objs={};
tri_name={};
operation='';
for t = 1:size(TRI,1)
    tri_objs(end+1)={geomcoerce('solid',{curve2(LOCVERTS(1,TRI(t,1:2)),LOCVERTS(2,TRI(t,1:2)),[1,1]), ...
                                        curve2(LOCVERTS(1,TRI(t,2:3)),LOCVERTS(2,TRI(t,2:3)),[1,1]), ...
                                        curve2(LOCVERTS(1,TRI(t,[3 1])),LOCVERTS(2,TRI(t,[3 1])),[1,1])})};
    ltri=strcat('TRI',num2str(t));
    tri_name(end+1)={ltri};
    if t>1
        operation=strcat(operation,'+');
    end
    operation=strcat(operation,ltri);
end
%geomplot(geomcoerce('solid',tri_objs));
%pause(.7);
%Fusion des triangles
s1=geomcomp(tri_objs,'ns',tri_name,'sf',operation,'edge','all');

%Transfert sur une geometrie 3D
p_workpln=[A;B;C];
obj=embed(s1,p_workpln');
%geomplot(obj);
%pause(1);