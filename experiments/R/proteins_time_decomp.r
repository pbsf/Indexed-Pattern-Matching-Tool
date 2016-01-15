mydata <- read.csv("data/proteins_time_decomp.raw", sep=" ")
pdf("pdf/proteins_time_decomp.pdf", height=4.5, family="Times")
matplot(mydata[, 1], mydata[, -1], type="l", xlab="Compressed file size in MB", ylab="Time in seconds")
legend("topleft", legend = colnames(mydata)[-1], fill=seq_along(colnames(mydata)[-1]))
