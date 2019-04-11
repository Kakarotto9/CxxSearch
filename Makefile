
CURR_ROOT=`pwd`


# Build cxxsearch
default:
	@g++ -o cxxsearch src/main.cpp

# Show makefile usage
help:
	@awk 'BEGIN{print "\nMakefile usage:\n"};/^[^#[:space:]\.].*:/&&$$0!~/=/{split($$0,t,":");printf("%8s %-16s %s\n","make",t[1],x);x=""};/^#/{gsub(/^# /,"");x=$$0;if(x!="")x="- "x};END{printf "\n"}' Makefile

# Build and run for tests
test: default
	@./cxxsearch -v -r "$(CURR_ROOT)/test/corpus/"

.PHONY: \
	default \
	help \
	test
