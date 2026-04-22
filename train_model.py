import pandas as pd
import numpy as np
from sklearn.svm import OneClassSVM
from micromlgen import port
import os


DATA_FILE = "dataset.csv"
HERKKYYS = 10.0        #  isompi luku = vähemmän herkkä
VIRHEMARGINAALI = 0.01 # kuinka monta prosenttia saa olla poikkeavuutta (0.01 = 1 %)
HALYTYSRAJA = -0.50    # milloin tulkintaan poikkeavaksi (isompi luku = herkempi)

def train():
    # katsotaan löytyykö tiedostoa ollenkaan
    if not os.path.exists(DATA_FILE):
        print(f"hups: {DATA_FILE} puuttuu")
        return

    df = pd.read_csv(DATA_FILE, header=None)
    X_raw = df.values
    
    # lasketaan sensorin poikkeama mediaanista
    medians = np.median(X_raw, axis=1, keepdims=True)
    X_features = np.abs(X_raw - medians)
    
    # lasketaan gamma-arvo datan perusteella
    variance = np.var(X_features)
    ideal_gamma = 1.0 / (X_features.shape[1] * variance) if variance > 0.01 else 0.001
    
    # säädetään mallin herkkyyttä
    relaxed_gamma = ideal_gamma / HERKKYYS
    
    #  opetetaan mallia tunnistamaan normaalit sensorien poikkeamat
    clf = OneClassSVM(kernel='rbf', gamma=float(relaxed_gamma), nu=VIRHEMARGINAALI)
    clf.fit(X_features)
    
    # c++ koodi conversion
    cpp_code = port(clf)
    
    # kirjaston bugien korjaukset
    cpp_code = cpp_code.replace("va_start(w, 3);", "va_start(w, x);")
    cpp_code = cpp_code.replace(" - ( + kernels", " + ( + kernels")
    
    # asetetaan hälytyskynnys
    cpp_code = cpp_code.replace("return decision > 0 ? 0 : 1;", f"return decision > {HALYTYSRAJA} ? 0 : 1;")
    
    # koodi include kansioon
    output_path = os.path.join("include", "model.h")
    with open(output_path, "w") as f:
        f.write(cpp_code)
        
 

