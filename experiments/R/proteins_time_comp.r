mydata <- read.csv("data/proteins_time_comp.raw", sep=" ")
pdf("pdf/proteins_time_comp.pdf", height=4.5, family="Times")
matplot(mydata[, 1], mydata[, -1], type="l", xlab="Original file size in MB", ylab="Time in seconds")
legend("topleft", legend = colnames(mydata)[-1], fill=seq_along(colnames(mydata)[-1]))
