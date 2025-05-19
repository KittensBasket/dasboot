#include <dasboot/cru/common.hpp>
#include <dasboot/cru/os.hpp>

#include <curl/curl.h>
#include <archive.h>
#include <archive_entry.h>

namespace NImageManager {
    using TStatus = NCommon::TStatus;

    enum Images {
        Alpine
    };

    class TImageManager {
        std::string ImagesLocation = "/var/lib/dasboot/images/";
        std::string CurrentImageName;

        public:
            TStatus Install(Images image);

        private:
            TStatus DownloadImage(Images image);
            TStatus DearchiveImage();
    };
} // namespace NImageManager
