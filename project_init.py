import os
import requests

root_path = ''
bin_path = 'bin'

bin_url = {'chi_tra.traineddata': 'https://github.com/tesseract-ocr/tessdata_best/raw/main/chi_tra.traineddata',
           'eng.traineddata': "https://github.com/tesseract-ocr/tessdata_best/raw/main/eng.traineddata"}
# https://github.com/tesseract-ocr/tessdata/raw/main/chi_tra.traineddata

# init project path
def path_search():
    global root_path
    global bin_path
    root_path = os.getcwd()
    bin_path = os.path.join(root_path, bin_path)


def download_bin_file():
    # check folder is exists
    if not os.path.exists('bin'):
        os.mkdir("bin")

    # download all need file
    for file_name, url in bin_url.items():

        #  check file is not exists
        if os.path.exists(os.path.join(bin_path, file_name)):
            print("File \'%s\' is download in path \'%s\'" % (file_name, bin_path))
            continue

        # if not exists download file
        print("Download the file \'%s\' to path \'%s\'" % (file_name, bin_path))
        file = requests.get(url, stream=True)
        with open(os.path.join(bin_path, file_name), "wb") as pdf:
            for chunk in file.iter_content(chunk_size=1024):
                if chunk:
                    pdf.write(chunk)


if __name__ == "__main__":
    path_search()
    download_bin_file()
