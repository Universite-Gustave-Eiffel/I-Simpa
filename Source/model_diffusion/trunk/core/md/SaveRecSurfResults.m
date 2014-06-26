function f = SaveRecSurfResults(rec_surfs,fem,idfreq)
%SaveRecSurfResults Exporte sous forme de fichiers texte les valeurs des
%points contenus dans rec_surfs
rootfold=strcat(rec_surfs.folderrsurf,rec_surfs.freqlbl{idfreq});
for idrs=1:size(rec_surfs.faces,2)
    arrvalues=[];
    recsfilename=strcat(rootfold,strcat(num2str(rec_surfs.names(idrs)),'.rsurf.txt'));
    if isfield(fem.sol,'tlist')
        if size(fem.appl,2)>1
            for idface=1:size(rec_surfs.faces{idrs},2)
                arrvalues=[arrvalues, postinterp(fem,sprintf('cel^2*%s*10^-12',rec_surfs.appl{idrs}{idface}),rec_surfs.faces{idrs}(:,idface),'T',fem.sol.tlist(2:size(fem.sol.tlist,2)))];
            end
        else
            arrvalues=postinterp(fem,'cel^2*u*10^-12',rec_surfs.faces{idrs},'T',fem.sol.tlist(2:size(fem.sol.tlist,2))); %ne pas exporter t0
        end
    else
        if size(fem.appl,2)>1
            for idface=1:size(rec_surfs.faces{idrs},2)
                arrvalues=[arrvalues, postinterp(fem,sprintf('cel^2*%s*10^-12',rec_surfs.appl{idrs}{idface}),rec_surfs.faces{idrs}(:,idface))];
            end
        else
            arrvalues=postinterp(fem,'cel^2*u*10^-12',rec_surfs.faces{idrs});
        end
    end
    save(recsfilename,'arrvalues','-ascii','-double');
end
f=1;