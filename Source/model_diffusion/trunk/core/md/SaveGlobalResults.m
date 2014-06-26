function f = SaveGlobalResults(fem,freqlbl)
%SaveGlobalResults Exporte le niveau pour
glbfilename=strcat(freqlbl,'.global.txt');
globalarrvalues=[];
for iapp = 1:size(fem.appl,2)
    app_domains=[];
    dom_index=find(cell2mat(fem.appl{iapp}.equ.usage) == 1);
    for domi = 1:size(dom_index,2)
       app_domains=[app_domains, find( fem.appl{iapp}.equ.ind == dom_index(domi) )];
    end
    if isfield(fem.sol,'tlist')
            appnum=num2str(iapp);
            if iapp==1
               appnum=''; 
            end
            globalarrvalues=[globalarrvalues;postint(fem,sprintf('rho*cel^2*u%s*10^-12',appnum),'Dl',app_domains,'T',fem.sol.tlist(2:size(fem.sol.tlist,2)))]; %ne pas exporter t0    
    else
            appnum=num2str(iapp);
            if iapp==1
               appnum=''; 
            end
            globalarrvalues=[globalarrvalues;postint(fem,sprintf('rho*cel^2*u%s*10^-12',appnum),'Dl',app_domains)];
    end
end
save(glbfilename,'globalarrvalues','-ascii','-double');
f=1;