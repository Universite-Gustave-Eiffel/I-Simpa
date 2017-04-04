%x = 10*rand (50, 1);
%          y = 10*rand (size (x));
%          h = convhull (x, y);
%          [vx, vy] = voronoi (x, y);
%          plot (vx, vy, "-b", x, y, "o", x(h), y(h), "-g");
%          legend ("", "points", "hull");
          
          
     rand ("state",9);
     x = rand (30,1);
     y = rand (30,1);
     tri = delaunay (x, y);
     [vx, vy] = voronoi (x, y, tri);
     triplot (tri, x, y, "b");
     hold on;
     plot (vx, vy, "r");
