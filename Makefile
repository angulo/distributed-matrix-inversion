#Copyright (C) 2010 Javier Angulo, César Mora

#This file is part of Distributed Matrix Inversion

#Distributed Matrix Inversion is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
#Distributed Matrix Inversion is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
#You should have received a copy of the GNU General Public License along with Distributed Matrix Inversion.  If not, see <http://www.gnu.org/licenses/>. 
CC=g++
OBJ=obj/
BIN=bin/
SRC=src/
INC=include/
DOC=doc/slice/
DOCLTX=doc/dmi/
SLICEGEN=generated/
TEST=test/
CXXFLAGS=-I $(INC) -I $(SLICEGEN) -I $${ICE_HOME}/include/ -ggdb 
LDFLAGS=-L $${ICE_HOME}/lib64/ -l Ice -l IceUtil
SLICEDIR=slice/
DB=config/db/

all: dirs docs slice objs exe

objs : $(OBJ)MatrixInversion.o $(OBJ)StoreI.o $(OBJ)AluI.o $(OBJ)MasterI.o $(OBJ)ClientI.o
exe : $(BIN)MasterNode $(BIN)StoreNode $(BIN)AluNode $(BIN)Main $(BIN)Generator

docs : $(DOC) 
	slice2html --output-dir $(DOC) -I $${ICE_HOME}/slice/ $(SLICEDIR)*.ice
	cd $(DOCLTX); pdflatex DMI.tex
	cd $(DOCLTX); pdflatex DMI.tex


$(OBJ)/%I.o : $(SRC)%I.cpp 
	$(CC) -c $^ $(CXXFLAGS)
	@mv $(@F) $(OBJ)

$(OBJ)%.o : $(SRC)%.cpp
	$(CC) -c $^ $(CXXFLAGS)
	@mv $(@F) $(OBJ)

$(OBJ)%.o : $(SLICEGEN)%.cpp
	$(CC) -c $^ $(CXXFLAGS)
	@mv $(@F) $(OBJ)

$(BIN)MasterNode : $(OBJ)MatrixInversion.o $(OBJ)StoreI.o $(OBJ)MasterI.o $(OBJ)MasterNode.o
	$(CC) -o $@ $^ $(CXXFLAGS) $(LDFLAGS)

$(BIN)StoreNode : $(OBJ)MatrixInversion.o $(OBJ)StoreI.o $(OBJ)StoreNode.o
	$(CC) -o $@ $^ $(CXXFLAGS) $(LDFLAGS)

$(BIN)AluNode : $(OBJ)MatrixInversion.o $(OBJ)AluI.o $(OBJ)AluNode.o
	$(CC) -o $@ $^ $(CXXFLAGS) $(LDFLAGS)

$(BIN)Main : $(OBJ)MatrixInversion.o $(OBJ)Main.o $(OBJ)ClientI.o
	$(CC) -o $@ $^ $(CXXFLAGS) $(LDFLAGS)

$(BIN)Generator : $(SRC)Generator.cpp
	$(CC) -o $@ $^ $(CXXFLAGS)

$(BIN)% : $(SRC)%.cpp
	$(CC) -o $@ $^ $(CXXFLAGS) $(LDFLAGS)

$(SLICEGEN)%.cpp $(SLICEGEN)%.h : $(SLICEDIR)%.ice
	slice2cpp --output-dir $(SLICEGEN)  $^ -I $(SLICEDIR) -I $${ICE_HOME}/slice/

slice: $(SLICEGEN)MatrixInversion.cpp $(SLICEGEN)MatrixInversion.h 

dirs:
	@mkdir -p $(OBJ)
	@mkdir -p $(BIN)
	@mkdir -p $(SLICEGEN)
	@mkdir -p $(DOC)
	@mkdir -p $(DB)MasterNode $(DB)StoreNode $(DB)registry config/logs

clean:
	@echo "Cleaning..."
	@rm -rf $(OBJ)
	@rm -rf $(BIN)
	@rm -rf $(DOC)
	@rm -rf $(SLICEGEN)
	@rm -rf $(DB)
	@rm -rf ./config/logs
	@rm -rf $(DOCLTX)*.toc $(DOCLTX)*.log $(DOCLTX)*.aux
	@echo "Done!"	
