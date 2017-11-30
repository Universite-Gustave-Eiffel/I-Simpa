 %=====================================================================================
  % SURFACES ABSORPTION el2diTrsm{ib}
%=====================================================================================

 
 Abs_Mater=load(strcat(domaine,'_materials_absorption.txt'));
 %iAbs: Indice des matériaux Absorbants 
iAbs=Abs_Mater( find(sum(Abs_Mater(:,2:end),2)));   
 [iAM,~]=find(sum(Abs_Mater(:,2:end),2)) ;
 iAbs=Abs_Mater(iAM,1);
abs_prop=Abs_Mater(iAM,:);
abs_coef=abs_prop(:,[BdOct1:BdOctend-1]);
      el2di_Dof=el2di;
 for iB=1:NBLOCKS
         [SurFaceA(iB),AireFace{iB}]= Surfaces_Salle(x,y,z,size(el2di{iB},1),el2di{iB});
        el2di_Dof{iB}(:,1:3)= changem(el2di{iB}(:,1:3), N_F{iB}(:,2), N_F{iB}(:,1));
        el2di_Dof{iB}(:,4)= changem(el2di_Dof{iB}(:,4),[1:size( iAbs,1)]' , iAbs ); 
          DofAbs{iB}=unique(el2di_Dof{iB}(:,1:3));
            
          for it=1:max(size(DofAbs{iB}))
                 [ligne,col] = find((el2di_Dof{iB}(:,1:3)-DofAbs{iB}(it))==0);
                % Surf_ech(it)=sum(AireFace{i}(ligne))/3;    
                 
         EquivAbsArea (it,1:NOct) = sum(   AireFace{iB}(ligne).*  abs_coef( el2di_Dof{iB}(ligne,4),1:NOct),1)/3;

 %       PondEchSurf (it,1:NOct) = sum(   AireFace{i}(ligne).*  TL100Hz5k(FacesTij{i}(ligne,4),1:end),1)/3;
                idfFree= DofAbs{iB}(it);
           for nm=1:NOct
           mat_Toct100Hz_5k{nm}( idfFree,  idfFree)= mat_Toct100Hz_5k{nm}( idfFree,  idfFree)...
           +EquivAbsArea (it,nm)*c0/4;
          end
             EquivAbsAreaBlock{iB}=sum    ( EquivAbsArea,1);
          end          
 end

%=================== Surfaces d'Absorption ===========================================