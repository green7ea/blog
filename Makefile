.PHONY: all clean

OUT = README.html

all: $(OUT)

%.html: %.md
	pandoc -s -c style.css -o $@ $<

clean:
	rm -f $(OUT)
