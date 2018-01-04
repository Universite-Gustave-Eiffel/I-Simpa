import math
def bluefilter(val):
    if val<.25 or val>.75:
        return 0.
    elif val<.5:
        return (val*4)-1
    else:
        return (-val*4)+3
    
def zeroifneg(val):
    if val<0:
        return 0
    else:
        return val
outfile=open("rvb.gpl",'w')
sizeof=255.
outfile.write("GIMP Palette\n")
outfile.write("Name: RVB %i\n" % (sizeof))
outfile.write("#\n")
for i in range(sizeof-1,-1,-1):
    perc=i/(sizeof-1)
    outfile.write("%i %i %i #%i\n" % (int(255*zeroifneg(1-(perc*2))),int(255*bluefilter(perc)),int(255*zeroifneg((perc*2)-1)),i))
outfile.close()
