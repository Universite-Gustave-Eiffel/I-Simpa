function f = BuildSubDomainsStructures(st,vol)
%BuildSubDomainsStructures Génére une structure de données des equations de sous-domaines en
%fonction des propriétés de volumes passés en paramètre et de la matrice de translation des domaines
clear equ; 
equ=vol;
clear equ.assoc;
%Création de equ.ind
equ.ind=ones(1,size(st,1));
%Affectation des equations propres aux domaines de la scène
for idsolid=1:size(vol.assoc,2)
    %equ.ind(find(st(:,idsolid)==1))=vol.assoc{idsolid};
    %Ignorer les volumes container (par ex la salle ne doit pas ecraser le
    %volume de la source
    res=find(st(:,idsolid)==1);
    if size(res,1)>1
      for idres=1:size(res,2)
        if size(find(st(res(idres),:)==1),2)==1
            res=res(idres);
            break;
        end
      end
    end
    equ.ind(res)=vol.assoc{idsolid};
end
f=equ;
