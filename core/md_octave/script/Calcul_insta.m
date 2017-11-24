%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%       SOLUTION INSTATIONNAIRE PARTANT DU REGIME ETABLI -
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%     Tableau des Alpha de chaque face en condition limite de 100Hz à 5kHz
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

for n=1:nel2D
        idm= find((abs_prop(:,1)-face_mat(n))==0);% indice matériau
        alpha_el2D=abs_prop(idm,5:21);% Tableau des Alpha de chaque face en condition limite de 100Hz à 5kHz
 end
               
Aire_Abs_eq=sum(alpha_el2D.*AireTr);
alphamoy=Aire_Abs_eq/Surf;% Coeff moyen SABINE
TR60=0.16*VOLUME./(Surf*alphamoy);% Tableau des TR selon SABINE
TR60E=0.16*VOLUME./(-Surf*log(1-alphamoy));% Tableau des TR selon  EYRING
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

dt=1e-3;% Pas de temps
itmax=round(2*TR60/3/dt);% Nombre d'iterations 

%[MATinsta]= integVC_lin(x,y,z,nn,nbel,el);% intï¿½gration lineaire
[MATinsta]= integVC_cst(nn,V_VC);% integration const
%========================================================================================================================================================
[wi_saved]= Euler_implicite(w100,mat_100,MATinsta,itmax(1),dt);
%========================================================================================================================================================
% Ecriture des résultats par bande de 1/3 octave
 ChpsInsta=strcat(domaine,'_WInstaFields100Hz');    
ChpsInsta2=strcat(ChpsInsta,'.hdf5');
 xx.a=dt;
 xx.b=wi_saved;
 save( '-hdf5', ChpsInsta2, 'xx')
'=========== FIN INSTA========='

%========================================================================================================================================================
[wi_saved]= Euler_implicite(w125,mat_125,MATinsta,itmax(2),dt);
%========================================================================================================================================================
% Sauvegarde de champs toutes les 0.05s (50 itérations avec dt=1e-3 s)
% Ecriture des résultats par bande de 1/3 octave
ChpsInsta=strcat(domaine,'_WInstaFields125Hz');    
ChpsInsta2=strcat(ChpsInsta,'.hdf5'); 
 xx.a=dt;
 xx.b=wi_saved;
 save( '-hdf5', ChpsInsta2, 'xx')
 %========================================================================================================================================================
[wi_saved]= Euler_implicite(w160,mat_160,MATinsta,itmax(3),dt);
%========================================================================================================================================================
% Sauvegarde de champs toutes les 0.05s (50 itérations avec dt=1e-3 s)
% Ecriture des résultats par bande de 1/3 octave
ChpsInsta=strcat(domaine,'_WInstaFields160Hz');    
ChpsInsta2=strcat(ChpsInsta,'.hdf5'); 
 xx.a=dt;
 xx.b=wi_saved;
 save( '-hdf5', ChpsInsta2, 'xx')
 %========================================================================================================================================================
[wi_saved]= Euler_implicite(w200,mat_200,MATinsta,itmax(4),dt);
%========================================================================================================================================================
% Sauvegarde de champs toutes les 0.05s (50 itérations avec dt=1e-3 s)
% Ecriture des résultats par bande de 1/3 octave
ChpsInsta=strcat(domaine,'_WInstaFields200Hz');    
ChpsInsta2=strcat(ChpsInsta,'.hdf5'); 
 xx.a=dt;
 xx.b=wi_saved;
 save( '-hdf5', ChpsInsta2, 'xx')

 %========================================================================================================================================================
[wi_saved]= Euler_implicite(w250,mat_250,MATinsta,itmax(5),dt);
%========================================================================================================================================================
% Sauvegarde de champs toutes les 0.05s (50 itérations avec dt=1e-3 s)
% Ecriture des résultats par bande de 1/3 octave
ChpsInsta=strcat(domaine,'_WInstaFields250Hz');    
ChpsInsta2=strcat(ChpsInsta,'.hdf5'); 
 xx.a=dt;
 xx.b=wi_saved;
 save( '-hdf5', ChpsInsta2, 'xx')

%========================================================================================================================================================
[wi_saved]= Euler_implicite(w315,mat_315,MATinsta,itmax(6),dt);
%========================================================================================================================================================
% Sauvegarde de champs toutes les 0.05s (50 itérations avec dt=1e-3 s)
% Ecriture des résultats par bande de 1/3 octave
ChpsInsta=strcat(domaine,'_WInstaFields315Hz');    
ChpsInsta2=strcat(ChpsInsta,'.hdf5'); 
 xx.a=dt;
 xx.b=wi_saved;
 save( '-hdf5', ChpsInsta2, 'xx')

 %========================================================================================================================================================
[wi_saved]= Euler_implicite(w400,mat_400,MATinsta,itmax(7),dt);
%========================================================================================================================================================
% Sauvegarde de champs toutes les 0.05s (50 itérations avec dt=1e-3 s)
% Ecriture des résultats par bande de 1/3 octave
ChpsInsta=strcat(domaine,'_WInstaFields400Hz');    
ChpsInsta2=strcat(ChpsInsta,'.hdf5'); 
 xx.a=dt;
 xx.b=wi_saved;
 save( '-hdf5', ChpsInsta2, 'xx')

'=========== FIN INSTA========='
%========================================================================================================================================================


% ANALYSE DE LA SOLUTION PAR RAPPORT A SABINE TR 
%LpdB=10*log10(wmic*rhoco2/4e-10);
%id= find(LpdB-LpdB(1)<-5);% On dÃ©bute apres 5dB de dÃ©croissance
%i_debut=id(1);
% 
%id= find(LpdB-LpdB(i_debut)<-20); 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%TRModel_Dif=(id(1)-i_debut)*dt*3
%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%t=0:dt:itmax*dt;
%%figure
%plot(t,wmic),xlabel('t (s)'),ylabel('Energie w')
%figure
%set(gca,'fontsize',18)
%hold on
%plot(t,LpdB),xlabel('t (s)'),ylabel('Niveau en dB'), grid on
%hold on
%plot([0 TR60/2],[LpdB(1) LpdB(1)-60/2],'r')
%
%plot([0 TR60E/2],[LpdB(1) LpdB(1)-60/2],'g')
%
% %legend('Modele diffusif', strcat('SABINE  : ',num2str(TR60),' s'), strcat('EYRING : ',num2str(TR60E),' s'))
%  legend(strcat('Modele diffusion',num2str(TRModel_Dif),' s'), strcat('SABINE  : ',num2str(TR60),' s'), strcat('EYRING : ',num2str(TR60E),' s'))
%
% title(strcat(  'TR Modele Diffusion  :',num2str(TRModel_Dif),' s'))
% 
% % Ecriture des résultats par bande de 1/3 octave
%   [fid] = fopen(filename,'w');
%   
%    fprintf(fid,w_insta(:));
% flose(fid)

   