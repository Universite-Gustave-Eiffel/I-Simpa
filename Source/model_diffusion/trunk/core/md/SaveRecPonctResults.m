function f = SaveRecPonctResults(rec_poncts,fem,idfreq)
%SaveRecPonctResults Exporte sous forme de fichiers texte les valeurs des
%points contenus dans rec_poncts
for idrp=1:size(rec_poncts.positions,2)
    recpfilename=strcat(rec_poncts.folderrponct,strcat(rec_poncts.names{idrp},strcat(rec_poncts.freqlbl{idfreq},'.rponct.txt')));
    if isfield(fem.sol,'tlist')
        arrvalues=postinterp(fem,sprintf('rho*cel^2*%s*10^-12',rec_poncts.appl{idrp}),rec_poncts.positions{idrp},'T',fem.sol.tlist(2:size(fem.sol.tlist,2))); %ne pas exporter t0
    else
        arrvalues=postinterp(fem,sprintf('rho*cel^2*%s*10^-12',rec_poncts.appl{idrp}),rec_poncts.positions{idrp});
    end
    save(recpfilename,'arrvalues','-ascii','-double');
end
f=1;