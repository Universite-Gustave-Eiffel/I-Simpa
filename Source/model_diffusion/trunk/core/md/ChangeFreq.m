function [paramout] = ChangeFreq(paramin,paramout,idfreq)
%ChangeFreq Met à jour les données pour passer à une autre bande de
%fréquence
for idchamp = 1:size(paramin,2)
    if(size(paramin{idchamp},2)>1 && not(ischar(paramin{idchamp})))
        paramout{idchamp}=paramin{idchamp}{idfreq};
    else
        paramout{idchamp}=paramin{idchamp};
    end
end