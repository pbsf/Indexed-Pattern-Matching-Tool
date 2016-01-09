mydata <- read.csv("data/comp_tax.csv", sep=" ")
pdf("pdf/comp_tax.pdf", height=4.5, family="Times")
matplot(mydata[, 1], mydata[, -1], type="l", xlab="Original file size in kbytes", ylab="Compression tax")
legend("topright", legend = colnames(mydata)[-1], fill=seq_along(colnames(mydata)[-1]))
