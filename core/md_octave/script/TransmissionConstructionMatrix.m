
%=====================================================================================
  % FACES TRANSMISSION el2diTrsm{ib}
%=====================================================================================
%i_nT= Tableau d'indices de Tous les noeud avec condition de transmission
i_nT=load(strcat(domaine,'_shared_vertices.txt'));i_nT=sort(i_nT);
Wall_TL=load(strcat(domaine,'_materials_transmission.txt'));
 %iTrsm: Indice des matériaux de transmission
iTrsm=Wall_TL( find(sum(Wall_TL(:,2:end),2)),1);
TL100Hz5k=Wall_TL( find(sum(Wall_TL(:,2:end),2)),[BdOct1:BdOctend-1]); % Transmission Loss idMat &values

TL100Hz5k(:,1:NOct)=10.^(-TL100Hz5k(:,1:NOct)/10);
for ib=1:NBLOCKS
el2diTrsm{ib}=el2di{ib}(ismember( el2di{ib}(:,end),iTrsm),:);
end
%figure
%affiche_triangle(x,y,z, el2diTrsm{1},'ob-'), hold on
%affiche_triangle(x,y,z, el2diTrsm{2},'xr-')
%affiche_triangle(x,y,z, el2diTrsm{3},'vg-')
it=0;
for i=1:NBLOCKS-1
el2Di=el2diTrsm{i};
nbParoiTransm=0;
for j=i+1:NBLOCKS
       
             el2Dj=el2diTrsm{j};
             Facescommunesij=el2Di(ismember(el2Di,el2Dj,'rows'),:);
           %  size(Facescommunesij)
             %Facescommunesij=unique(Facescommunesij,'rows');   size(Facescommunesij)
             cond=isempty(Facescommunesij);
     if not(cond)
                   it=it+1;
                   FacesTij{it}=Facescommunesij;
                    FacesTij{it}(:,4)= changem(FacesTij{it}(:,4),[1:size(iTrsm,1)]' ,iTrsm );
                  
                   FacesTi_Dof{it}= changem(FacesTij{it}(:,1:3), N_F{i}(:,2), N_F{i}(:,1));
                   FacesTi_Dof{it}(:,4)= FacesTij{it}(:,4);
                   FacesTj_Dof{it}= changem(FacesTij{it}(:,1:3), N_F{j}(:,2), N_F{j}(:,1));
                   FacesTj_Dof{it}(:,4)= FacesTij{it}(:,4);  
     end   
 end   
end
NbTransmWall=it

%========================================================================================
% surfaces d'échange (somme des surfaceS des triangles auxquels chaque point appartient/3 )

for i=1:NbTransmWall
          [SurFaceT(i),AireFace{i}]= Surfaces_Salle(x,y,z,size(FacesTij{i},1),FacesTij{i});
            ptsTrsm{i}=unique(FacesTij{i}(:,1:3));
            Dofi{i}=unique(FacesTi_Dof{i}(:,1:3));
            Dofj{i}=unique(FacesTj_Dof{i}(:,1:3));
  for it=1:max(size(ptsTrsm{i}))
           [ligne,col] = find((FacesTij{i}(:,1:3)-ptsTrsm{i}(it))==0);
           Surf_ech(it)=sum(AireFace{i}(ligne))/3;  
              % PondEchSurf (it) =sum(AireFace{i}(ligne).*TL100Hz5k(FacesTij{i}(ligne,4), 2)')/3;  
        PondEchSurf (it,1:NOct) = sum(   AireFace{i}(ligne).*  TL100Hz5k(FacesTij{i}(ligne,4),1:end),1)/3;
 %============================================================================================================================
 % Contribution de la TRANSMISSION
           for nm=1:NOct
           mat_Toct100Hz_5k{nm}(Dofi{i}(it), Dofj{i}(it))=mat_Toct100Hz_5k{nm}(Dofi{i}(it), Dofj{i}(it))...
           +PondEchSurf (it,nm) *c0/4;
          mat_Toct100Hz_5k{nm}(Dofj{i}(it), Dofi{i}(it))=mat_Toct100Hz_5k{nm}(Dofj{i}(it), Dofi{i}(it))...
           +PondEchSurf (it,nm) *c0/4;           
          end
%============================================================================================================================
 end          
 %         Surf_echgeT{i}=PondEchSurf;
          clear Surf_ech
          clear PondEchSurf
  end
