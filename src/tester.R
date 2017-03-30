# This script lets you convert between output vector values and real parameters
sol=c(0.3980255376244080,0.6666666848675786,0.0477462437361110)
signal = read.csv("signal.txt",header=FALSE)
x = 1:length(signal)

voltsConstantMin = 100;
voltsConstantMax = 300;					# 100-300V peak
omegaConstantMin = 2.0*pi*40;
omegaConstantMax = 2.0*pi*70;			# 40-70Hz
phiConstant = 2.0*pi;					# 0-2PI radians
rate = 600;

amplitude = voltsConstantMin+sol[1]*(voltsConstantMax - voltsConstantMin)
omega = omegaConstantMin+sol[2]*(omegaConstantMax - omegaConstantMin)
phi = sol[3]*phiConstant
y = amplitude*sin(omega*(x-1)/rate+phi)
plot(x,y,type='l',col='blue')
lines(x,signal,type='l',col='red')