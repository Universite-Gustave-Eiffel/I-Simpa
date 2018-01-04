## INPUT PARAMETERS THAT SHOULD BE READ FROM I-Simpa CONFIG FILE

# Frequency bands selection (from 'Frequency bands')
TOB=[50, 63, 80, 100, 125, 160, 200, 250, 320, 400, 500, 640, 800, 1000, 1250, 1600, 2000, 2500, 3200, 4000, 5000, 6300, 8000, 10000, 12500, 16000, 20000]; # All possible bands (should be the same that within I-Simpa
BdOct1=4; # First selected frequency band for calculation
BdOctend=21; # Last selected frequency band for calculation

# Environmental parametersTOB
Temperature=20; # Temperature (°C)
Humidity=50; # Relative humidity (%)
Pressure=101325; # Air pressure (Pa)

# Calculation parameters (from 'Properties')
tol=1e-6; # 'Diffusion equation resolution: Tolerance' (bicgstab function; default should be: 1e-6)
maxint=200 # 'Diffusion equation resolution: Maximum number of iterations' (default should be: 200)
dt=0.01; # 'Time step (s)' for the sound decay calculation
duration=2; # 'Simulation length (s)' for the sound decay calculation
atmos_absorption_calculation=1; # If 0 no atmospheric calculation;


