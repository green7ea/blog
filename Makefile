.PHONY: all clean

all:
	$(MAKE) -C modern-cpp/src/
	$(MAKE) -C smart-ptr/src/

clean:
	$(MAKE) -C modern-cpp/src/ clean
	$(MAKE) -C smart-ptr/src/ clean
