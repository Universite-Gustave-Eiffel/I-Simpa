               function M= matrice_transmission(M,ks,ks2,TauxAireTr3)
             

                M(ks2(1),ks(1))=M(ks2(1),ks(1))+TauxAireTr3; 
                M(ks2(2),ks(2))=M(ks2(1),ks(2))+TauxAireTr3;
                M(ks2(3),ks(3))=M(ks2(3),ks(3))+TauxAireTr3;
                M(ks(1),ks2(1))=M(ks(1),ks2(1))+TauxAireTr3; 
                M(ks(2),ks2(2))=M(ks(2),ks2(2))+TauxAireTr3;
                M(ks(3),ks2(3))=M(ks(3),ks2(3))+TauxAireTr3;
                
                end