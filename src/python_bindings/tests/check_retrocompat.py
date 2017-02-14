from __future__ import print_function
import os
import sys
sys.path.append(os.getcwd())
import libsimpa as ls

expected_faces = [[16, 27, 22, -1, 101, -1], [19, 27, 16, -1, 101, -1], [4, 11, 2, -1, 100, -1], [11, 10, 2, -1, 100, -1], [19, 1, 37, -1, 100, -1], [14, 9, 18, -1, 100, -1], [0, 41, 13, -1, 100, -1], [7, 12, 41, -1, 100, -1], [15, 40, 8, -1, 100, -1], [23, 40, 15, -1, 100, -1], [18, 28, 14, -1, 100, -1], [33, 22, 10, -1, 101, -1], [24, 23, 41, -1, 100, -1], [33, 32, 22, -1, 101, -1], [24, 35, 16, -1, 101, -1], [20, 14, 4, -1, 100, -1], [24, 41, 0, -1, 100, -1], [10, 11, 31, -1, 100, -1], [10, 31, 33, -1, 100, -1], [9, 19, 18, -1, 100, -1], [18, 37, 6, -1, 100, -1], [12, 37, 13, -1, 100, -1], [13, 37, 1, -1, 100, -1], [10, 22, 20, -1, 101, -1], [16, 22, 8, -1, 101, -1], [9, 14, 20, -1, 100, -1], [21, 15, 30, -1, 100, -1], [23, 7, 41, -1, 100, -1], [12, 13, 41, -1, 100, -1], [5, 39, 29, -1, 100, -1], [16, 8, 24, -1, 101, -1], [12, 6, 37, -1, 100, -1], [0, 35, 24, -1, 101, -1], [18, 26, 17, -1, 100, -1], [13, 35, 0, -1, 101, -1], [15, 17, 23, -1, 100, -1], [2, 20, 4, -1, 100, -1], [10, 20, 2, -1, 101, -1], [15, 21, 17, -1, 100, -1], [31, 11, 21, -1, 100, -1], [31, 21, 30, -1, 100, -1], [8, 40, 24, -1, 100, -1], [33, 31, 29, -1, 100, -1], [17, 28, 18, -1, 100, -1], [21, 28, 17, -1, 100, -1], [18, 19, 37, -1, 100, -1], [16, 25, 19, -1, 101, -1], [1, 25, 13, -1, 101, -1], [25, 1, 19, -1, 101, -1], [6, 26, 18, -1, 100, -1], [26, 6, 12, -1, 100, -1], [9, 27, 19, -1, 101, -1], [20, 27, 9, -1, 101, -1], [22, 27, 20, -1, 101, -1], [14, 28, 4, -1, 100, -1], [11, 28, 21, -1, 100, -1], [4, 28, 11, -1, 100, -1], [5, 31, 30, -1, 100, -1], [29, 31, 5, -1, 100, -1], [24, 40, 23, -1, 100, -1], [32, 8, 22, -1, 101, -1], [32, 33, 3, -1, 101, -1], [3, 33, 29, -1, 100, -1], [17, 34, 23, -1, 100, -1], [26, 34, 17, -1, 100, -1], [12, 34, 26, -1, 100, -1], [7, 34, 12, -1, 100, -1], [34, 7, 23, -1, 100, -1], [25, 35, 13, -1, 101, -1], [16, 35, 25, -1, 101, -1], [30, 39, 5, -1, 100, -1], [15, 39, 30, -1, 100, -1], [3, 39, 32, -1, 100, -1], [29, 39, 3, -1, 100, -1], [8, 39, 15, -1, 100, -1], [32, 39, 8, -1, 100, -1]]

def check_file(fileName):
    model_reader = ls.CformatBIN()

    model = ls.ioModel()
    if model_reader.ImportBIN(model, fileName):
        # Import success
        # read content
        assert(len(model.faces) == len(expected_faces))
        faces = [[f.a, f.b, f.c, f.idEn, f.idMat, f.idRs] for f in model.faces]
        for refFace in expected_faces:
            if not refFace in faces:
                print("Got face ref {0} not found in model" % (refFace))
        for modelFace in faces:
            if not modelFace in expected_faces:
                print("Got face {0} not found in ref model" % (modelFace))
        assert(len(model.vertices) == 42)
        vertices = [(v[0], v[1], v[2]) for v in model.vertices]
        # compute min max for x,y,z
        maxV = map(max, zip(*vertices))
        minV = map(min, zip(*vertices))
        assert(abs(maxV[0] - 20) < 1e-5)
        assert(abs(maxV[1] - 30) < 1e-5)
        assert(abs(maxV[2] - 10) < 1e-5)
        
        assert(abs(minV[0]) < 1e-5)
        assert(abs(minV[1]) < 1e-5)
        assert(abs(minV[2]) < 1e-5)
        return model
    else:
        print("Test failed with %s" % fileName)
        exit(-1)
        
# Open old file and check it       
model = check_file(sys.argv[1])
# Save the content to another file
model_writer = ls.CformatBIN()
expfile = sys.argv[2]+"test_retrocompat.cbin"
model_writer.ExportBIN(expfile, model)
# Check the content of new file
check_file(expfile)
print("Test sucess")