a=read.csv("myDiffSignal.txt",header=FALSE)
plot(1:length(a),a,type='l')
abline(h=0.05*127*sqrt(2))
abline(h=-0.05*127*sqrt(2))