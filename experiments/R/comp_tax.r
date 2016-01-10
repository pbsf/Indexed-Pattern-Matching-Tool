mydata <- read.csv("data/comp_tax.csv", sep=" ")
pdf("pdf/comp_tax.pdf", height=4.5, family="Times")
matplot(mydata[, 1], mydata[, -1], type="l", xlab="Original file size in MB", ylab="Compression tax")
legend("bottomright", legend = colnames(mydata)[-1], fill=seq_along(colnames(mydata)[-1]))
