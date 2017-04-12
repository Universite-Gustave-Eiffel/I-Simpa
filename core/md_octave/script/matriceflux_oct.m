function [mat_octave]= matriceflux_oct(ai,ajk, ks,mat_octave)
         
             
                   mat_octave(ks(1),ks(2))=ajk+mat_octave(ks(1),ks(2));
                   mat_octave(ks(1),ks(3))=ajk+mat_octave(ks(1),ks(3));
                   mat_octave(ks(1),ks(1))=ai+mat_octave(ks(1),ks(1));

                   mat_octave(ks(2),ks(1))=ajk+mat_octave(ks(2),ks(1));
                   mat_octave(ks(2),ks(3))=ajk+mat_octave(ks(2),ks(3));
                   mat_octave(ks(2),ks(2))=ai+mat_octave(ks(2),ks(2));
              
                   mat_octave(ks(3),ks(1))=ajk+mat_octave(ks(3),ks(1));
                   mat_octave(ks(3),ks(2))=ajk+mat_octave(ks(3),ks(2));
                   mat_octave(ks(3),ks(3))=ai+mat_octave(ks(3),ks(3));
   end