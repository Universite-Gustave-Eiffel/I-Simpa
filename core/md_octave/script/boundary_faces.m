
function F = boundary_faces(T)
  % F = boundary_faces(T)
  % Détermine les faces des Tetrahedres T 
  % en frontieres du domaine
  %
  % ENTREE:
  %  T  , m * 4, où m est le nombre de tetrahedres
  %
  % Output:
  %  F  liste des faces en frontiere, n*3, n nombre de triangles frontiere
  %

  % On recupere toutes les faces des TETRA
  allF = [ ...
    T(:,1) T(:,2) T(:,3); ...
    T(:,1) T(:,3) T(:,4); ...
    T(:,1) T(:,4) T(:,2); ...
    T(:,2) T(:,4) T(:,3)];
size(allF)
  % Tri des faces par ordre croissant des indices
  sortedF = sort(allF,2);
  % determine unicite des faces
  [u,m,n] = unique(sortedF,'rows');
  % determine counts for each unique face
  counts = accumarray(n(:), 1);
  % extract faces that only occurred once
  sorted_exteriorF = u(counts == 1,:);
  % find in original faces so that ordering of indices is correct
  F = allF(ismember(sortedF,sorted_exteriorF,'rows'),:);
end