function PlotFieldonMesh(coordinates,nodes,component)
%--------------------------------------------------------------------------
% Purpose:
%         To plot the profile of a component on mesh
% Synopsis :
%           ProfileonMesh(coordinates,nodes,component)
% Variable Description:
%           coordinates - The nodal coordinates of the mesh
%           -----> coordinates = [X Y Z] 
%           nodes - The nodal connectivity of the elements
%           -----> nodes = [node1 node2......]      
%           component - The components whose profile to be plotted
%           -----> components = a column vector in the order of node
%                               numbers
%
% Coded by :    Siva Srinivas Kolukula, PhD      
%               Indian Tsunami Early Warning Centre (ITEWC)
%               Advisory Services and Satellite Oceanography Group (ASG)
%               Indian National Centre for Ocean Information Services (INCOIS)
%               Hyderabad, INDIA
% E-mail   :    allwayzitzme@gmail.com                                        
% web-link :    https://sites.google.com/site/kolukulasivasrinivas/   
%
% version 1: 28 August 2011
% Version 2: 16 September 2016
%--------------------------------------------------------------------------

dimension = size(coordinates,2) ;  % Dimension of the mesh
nel = length(nodes) ;                  % number of elements
nnode = length(coordinates) ;          % total number of nodes in system
nnel = size(nodes,2);                % number of nodes per element
% 
% Initialization of the required matrices
X = zeros(nnel,nel) ;
Y = zeros(nnel,nel) ;
Z = zeros(nnel,nel) ;
profile = zeros(nnel,nel) ;
%
if dimension == 3   % For 3D plots
    if nnel == 4  % Surface in 3D 
        for iel=1:nel   
            nd=nodes(iel,:);         % extract connected node for (iel)-th element
            X(:,iel)=coordinates(nd,1);    % extract x value of the node
            Y(:,iel)=coordinates(nd,2);    % extract y value of the node
            Z(:,iel)=coordinates(nd,3) ;   % extract z value of the node
            profile(:,iel) = component(nd') ; % extract component value of the node 
        end
        % Plotting the FEM mesh and profile of the given component
        figure
        fill3(X,Y,Z,profile)
        rotate3d on ;
        title('Profile of component on Mesh') ;         
        axis off ; 
        % Colorbar Setting
      %  SetColorbar
    elseif nnel == 8  % solid in 3D
        fm = [1 2 6 5; 2 3 7 6; 3 4 8 7; 4 1 5 8; 1 2 3 4; 5 6 7 8];
        XYZ = cell(1,nel) ;
        profile = XYZ ;
        for e=1:nel
            nd=nodes(e,:);
            X = coordinates(nd,1) ;
            Y = coordinates(nd,2) ;
            Z = coordinates(nd,3) ;
            XYZ{e} = [X  Y Z] ;
            profile{e} = component(nd) ;
        end
        figure
        cellfun(@patch,repmat({'Vertices'},1,nel),XYZ,.......
            repmat({'Faces'},1,nel),repmat({fm},1,nel),......
            repmat({'FaceVertexCdata'},1,nel),profile,......
            repmat({'FaceColor'},1,nel),repmat({'interp'},1,nel));
        view(3)
        set(gca,'XTick',[]) ; set(gca,'YTick',[]); set(gca,'ZTick',[]) ;
        % Colorbar Setting
        SetColorbar
        
    end
elseif dimension == 2           % For 2D plots
    
    for iel=1:nel   
        nd=nodes(iel,:);         % extract connected node for (iel)-th element
        X(:,iel)=coordinates(nd,1);    % extract x value of the node
        Y(:,iel)=coordinates(nd,2);    % extract y value of the node
        profile(:,iel) = component(nd') ;         % extract component value of the node 
    end
    % Plotting the FEM mesh and profile of the given component
    figure
    % plot(X,Y,'k')
    fill(X,Y,profile)
    title('Profile of component on Mesh') ;         
    axis off ;
    % Colorbar Setting
    SetColorbar
end

              
         
 
   
     
       
       
