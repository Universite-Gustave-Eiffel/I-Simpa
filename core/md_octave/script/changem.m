function mapout = changem(Z, newcode, oldcode)
% Idential to the Mapping Toolbox's changem
% Note the weird order: newcode, oldcode. I left it unchanged from Matlab.
    if numel(newcode) ~= numel(oldcode)
        error('newcode and oldcode must be equal length');
    end

    mapout = Z;

    for ii = 1:numel(oldcode)
        mapout(Z == oldcode(ii)) = newcode(ii);
    end
end