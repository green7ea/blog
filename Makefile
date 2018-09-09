.PHONY: all clean

all: blog.html

%.html: %.md
	pandoc -s -c style.css -o $@ $<

clean:
	rm -f blog.html
