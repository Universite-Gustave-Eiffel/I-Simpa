%   % Tet mesh in (V,T)
T=el;V=w;
    F = boundary_faces(T);
  %   % Solve poisson equation for interesting function inside
     L = cotmatrix(V,T);
     M = massmatrix(V,T);
     b = unique(F);
  %   int = setdiff(1:size(V,1),b);
  %   H = zeros(size(V,1),1);
  %   H(int) = (-L(int,int))\(M(int,int)*ones(numel(int),1));
  %   clf;
  %   t = tsurf(F,V,'FaceColor',[0.8 0.8 0.8],'FaceAlpha',0.2,'EdgeAlpha',0.2);
  %   hold on;
  %     s = tsurf([1 1 1],V,'EdgeColor','none',fphong);
  %     BB = bounding_box(V(:,1:2));
  %     BB = BB([1 2 4 3 1],:);
  %     p = plot3(BB(:,1),BB(:,2),min(V(:,3))*[1;1;1;1;1],'-','LineWidth',3);
  %   hold off;
  %   caxis([min(H) max(H)]);
  %   axis equal;
  %   for z = linspace(min(V(:,3)),max(V(:,3)))
  %     [U,G,BC] = slice_tets(V,T,[0 0 1 -z]);
  %     set(s,'Vertices',U,'Faces',G,'CData',BC*H);
  %     p.ZData = z*[1;1;1;1;1];
  %     drawnow;
  %   end
  %
