function f = BuildBoundaryStructure(ft,mat)
%BuildBoundaryStructure Génére une structure de données de Limites en
%fonction des matériaux passés en paramètre et de la matrice de translation
clear bnd;
bnd=mat.materials;
%Création de bnd.ind
bnd.ind=ones(1,size(ft,1));
%Affectation des matériaux de la scène
for idsolid=1:size(mat.faces,2)
    id=1;
    for i=mat.faces{idsolid}
       bnd.ind(find(ft(:,i)==1))=mat.assoc{idsolid}(id);
       id=id+1;
    end
end
f=bnd;