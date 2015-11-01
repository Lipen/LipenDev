draw <- function(name) {
	loc <- cmdscale(dist(as.matrix(read.delim(name, sep = " ", header = F))))
	x <- loc[,1]
	y <- -loc[,2]
	plot(x, y, type = "p", xlab = "", ylab = "", asp = 1, axes = T, main = paste("cmdscale(", name, ")", sep = ""))
}
