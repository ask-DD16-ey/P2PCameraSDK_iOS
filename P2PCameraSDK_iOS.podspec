#
#  Be sure to run `pod spec lint P2PCameraSDK_iOS.podspec' to ensure this is a
#  valid spec and to remove all comments including this before submitting the spec.
#
#  To learn more about Podspec attributes see https://guides.cocoapods.org/syntax/podspec.html
#  To see working Podspecs in the CocoaPods repo see https://github.com/CocoaPods/Specs/
#

Pod::Spec.new do |spec|

  # ―――  Spec Metadata  ―――――――――――――――――――――――――――――――――――――――――――――――――――――――――― #
  #
  #  These will help people to find your library, and whilst it
  #  can feel like a chore to fill in it's definitely to your advantage. The
  #  summary should be tweet-length, and the description more in depth.
  #

  spec.name         = "P2PCameraSDK_iOS"
  spec.version      = "1.0.18.beta.2"
  spec.summary      = "Camera SDK used on iOS"

  spec.description  = <<-DESC
                   It is a Camera SDK used on iOS, which implement by Objective-C.
                   DESC
  spec.homepage     = "https://github.com/ask-DD16-ey/P2PCameraSDK_iOS"
  spec.license      = "MIT"
  spec.author             = { "ask-DD16-ey" => "askey.dd16@gmail.com" }
  spec.platform     = :ios, "10.0"
  spec.source       = { :git => "https://github.com/ask-DD16-ey/P2PCameraSDK_iOS.git", :tag => "#{spec.version}" }
  spec.source_files  = "CameraSDK.framework/Headers/*.{h}"
  #spec.requires_arc = true
  spec.vendored_frameworks = 'CameraSDK.framework'
  spec.frameworks = 'Foundation'
  spec.libraries = 'z.1.2.11', 'iconv.2.4.0', 'bz2.1.0'
end
