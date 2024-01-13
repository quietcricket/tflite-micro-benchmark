from PIL import Image

def convert(filename):
	im=Image.open(filename).convert('L')
	pixels = list(im.getdata()) 
	print(pixels)


convert("0001.bmp")