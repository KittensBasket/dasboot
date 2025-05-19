#include "imagemanager.hpp"

namespace NImageManager {

    TStatus TImageManager::Install(Images image) {
        TStatus status;

        status = DownloadImage(image);
        if (status.Code == TStatus::Failed) {
            status.Error = "TImageManager::Install: " + status.Error;
            return status;
        }

        status = DearchiveImage();
        return { TStatus::Success };
    }

    // GOD FORBID US - curl requires C-styled function with this signature
    // TBD - move it to os.hpp - VoidZeroNull0
    size_t WriteArchive(void* ptr, size_t size, size_t nmemb, FILE* stream) {
        return fwrite(ptr, size, nmemb, stream);
    }

    TStatus TImageManager::DownloadImage(Images image) {
        TStatus status;

        CURL* curl = curl_easy_init();
        if (!curl) {
            status.Code = TStatus::Failed;
            status.Error = "TImageManager::DownloadImage:Could not init curl";
            return status;
        }

        std::string url;
        CurrentImageName = "";
        if (image == Images::Alpine) {
            url = "https://dl-cdn.alpinelinux.org/alpine/v3.21/releases/x86_64/alpine-minirootfs-3.21.3-x86_64.tar.gz";
            CurrentImageName = "alpine";
        }
        else {
            status.Code = TStatus::Failed;
            status.Error = "TImageManager::DownloadImage: Non-implemented image";
            return status; 
        }

        std::string path = MakeString() << ImagesLocation << CurrentImageName << ".tar.gz"; 

        FILE* file = fopen(path.c_str(), "wb");
        if (!file) {
            status.Code = TStatus::Failed;
            status.Error = "TImageManager::DownloadImage: Could not create file";
            return status;
        }

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteArchive);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        CURLcode result = curl_easy_perform(curl);
        if (result != CURLE_OK) {
            status.Code = TStatus::Failed;
            status.Error = MakeString() << "TImageManager::DownloadImage: Curl result error" << result;
            fclose(file);
            curl_easy_cleanup(curl);
            return status;
        }

        fclose(file);
        curl_easy_cleanup(curl);

        return { TStatus::Success };
    }

    TStatus TImageManager::DearchiveImage() {
        TStatus status;

        struct archive* a = archive_read_new();
        struct archive* ext = archive_write_disk_new();
        struct archive_entry* entry;

        archive_read_support_format_tar(a);
        archive_read_support_filter_gzip(a); // для .gz

        std::string path = MakeString() << ImagesLocation << CurrentImageName << ".tar.gz";
        if (archive_read_open_filename(a, path.c_str(), 10240)) {
            status.Code = TStatus::Failed;
            status.Error = "TImageManager::DownloadImage: Could not open archive";
            return status;
        }

        path = MakeString() << ImagesLocation << "/" << CurrentImageName;
        while (archive_read_next_header(a, &entry) == ARCHIVE_OK) {
            const char* currentFile = archive_entry_pathname(entry);
            std::string fullOutputPath = path + "/" + currentFile;
            archive_entry_set_pathname(entry, fullOutputPath.c_str());

            archive_write_header(ext, entry);
            const void* buff;
            size_t size;
            la_int64_t offset;

            while (archive_read_data_block(a, &buff, &size, &offset) == ARCHIVE_OK) {
                archive_write_data_block(ext, buff, size, offset);
            }
        }

        archive_read_close(a);
        archive_read_free(a);
        archive_write_close(ext);
        archive_write_free(ext);

        return { TStatus::Success };
    }
} // namespace NImageManager
