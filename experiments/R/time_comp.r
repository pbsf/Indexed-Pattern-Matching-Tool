mydata <- read.csv("data/time_comp.csv", sep=" ")
pdf("pdf/time_comp.pdf", height=4.5, family="Times")
matplot(mydata[, 1], mydata[, -1], type="l", xlab="Original file size in kbytes", ylab="Time in ms")
legend("topleft", legend = colnames(mydata)[-1], fill=seq_along(colnames(mydata)[-1]))
