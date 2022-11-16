import os
import requests
import shutil

root_path = ''
bin_path = 'bin'
dll_path = ''
model_path = 'model'
cmake_path = 'cmake'
libraries_path = 'libraries'
bin_url = {'chi_tra.traineddata': 'https://github.com/tesseract-ocr/tessdata_best/raw/main/chi_tra.traineddata',
           'eng.traineddata': "https://github.com/tesseract-ocr/tessdata_best/raw/main/eng.traineddata"}


# https://github.com/tesseract-ocr/tessdata/raw/main/chi_tra.traineddata

# init project path
def path_search():
    global root_path
    global bin_path
    global cmake_path
    global libraries_path
    global model_path
    global dll_path
    root_path = os.getcwd()
    bin_path = os.path.join(root_path, bin_path)
    cmake_path = os.path.join(root_path, cmake_path)
    libraries_path = os.path.join(root_path, libraries_path)
    model_path = os.path.join(bin_path, model_path)
    dll_path = os.path.join(bin_path, dll_path)


def download_bin_file():
    # check folder is exists
    if not os.path.exists('bin'):
        os.mkdir("bin")

    if not os.path.exists(model_path):
        os.mkdir('bin/model')

    # download all need file
    for file_name, url in bin_url.items():

        #  check file is not exists
        if os.path.exists(os.path.join(model_path, file_name)):
            print("File \'%s\' is download in path \'%s\'" % (file_name, model_path))
            continue

        # if not exists download file
        print("Download the file \'%s\' to path \'%s\'" % (file_name, model_path))
        file = requests.get(url, stream=True)
        with open(os.path.join(model_path, file_name), "wb") as pdf:
            for chunk in file.iter_content(chunk_size=1024):
                if chunk:
                    pdf.write(chunk)


def install_lib():
    os.chdir(libraries_path)

    # Xlnt Build

    os.system('git clone https://github.com/tfussell/xlnt.git')
    os.chdir(os.path.join(libraries_path, 'xlnt'))
    os.system('git submodule init')
    os.system('git submodule update')

    if not os.path.exists(os.path.join(libraries_path, 'xlnt', 'build')):
        os.mkdir('build')
    os.chdir(os.path.join(libraries_path, 'xlnt', 'build'))
    os.system('cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_BUILD_TYPE=Release ..')
    os.system('cmake --build .  -j12 --target install --config Release')

    if os.path.exists(os.path.join(cmake_path, 'xlnt')):
        shutil.rmtree(os.path.join(cmake_path, 'xlnt'), ignore_errors=True)

    shutil.copytree(os.path.abspath('installed'), os.path.join(cmake_path, 'xlnt'))

    if not os.path.exists(dll_path):
        os.mkdir(dll_path)

    for file_name in os.listdir((os.path.join(cmake_path, 'xlnt', 'bin'))):
        if not os.path.exists(os.path.join(dll_path, file_name)):
            shutil.copy(os.path.join(cmake_path, 'xlnt', 'bin', file_name), os.path.join(dll_path, file_name))


if __name__ == "__main__":
    path_search()
    download_bin_file()
    install_lib()
    input("Project Setup Finsh, Press any key to exit.")
