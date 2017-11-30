%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%       SOLUTION INSTATIONNAIRE PARTANT DU REGIME ETABLI -
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%     Tableau des Alpha de chaque face en condition limite de 100Hz à 5kHz
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

for iB=1:NBLOCKS

Aire_Abs_eq=  EquivAbsAreaBlock{iB};
%alphamoy=Aire_Abs_eq/Surf;% Coeff moyen SABINE
TR60{iB}=0.16*VOLUME(iB)./Aire_Abs_eq;% Tableau des TR de 100Hz-5kHz selon SABINE
%TR60E=0.16*VOLUME./(-Surf*log(1-alphamoy));% Tableau des TR selon  EYRING
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
end
 dt=1e-3;% Pas de temps
 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 
itmax=round(2*max(TR60{1})/3/dt);% Nombre d'iterations 
%itmax=5 *itmax;
%[MATinsta]= integVC_lin(x,y,z,nn,nbel,el);% integration lineaire
[MATinsta]= integVC_cst(V_VC);% integration const
%========================================================================================================================================================
oct3rd= [100, 125, 160, 200, 250, 320, 400, 500, 640, 800, 1000, 1250, 1600, 2000, 2500, 3200, 4000, 5000];
NomFichier=strcat(domaine,'_WInstaFields');  
tic 
for  N_Toct=1:NOct;

[wi_saved]= Euler_implicite(w{N_Toct},mat_Toct100Hz_5k{N_Toct},MATinsta,itmax(1),dt);

%[wi_saved]= Euler_explicite(w{N_Toct},mat_Toct100Hz_5k{N_Toct},MATinsta,itmax(1),dt/500);
%affichepatch

% Ecriture des résultats par bande de 1/3 octave

ChpsInsta=strcat(NomFichier,int2str(oct3rd(N_Toct)));   
ChpsInsta2=strcat(ChpsInsta,'.hdf5');
 xx.a=dt;
 xx.b=wi_saved;
 save( '-hdf5', ChpsInsta2, 'xx')
 end
%========================================================================================================================================================
toc
%'=========== FIN INSTA========='
%pastemporel_sauvegarde=dt;
