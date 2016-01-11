mydata <- read.csv("data/rq3.raw", sep=" ")
pdf("pdf/rq3.pdf", height=4.5, family="Times")
matplot(mydata[, 1], mydata[, -1], type="l", xlab="Pattern size", ylab="Time in seconds")
legend("bottomright", legend = colnames(mydata)[-1], fill=seq_along(colnames(mydata)[-1]))
