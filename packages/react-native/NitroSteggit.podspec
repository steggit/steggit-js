require "json"

package = JSON.parse(File.read(File.join(__dir__, "package.json")))

Pod::Spec.new do |s|
  s.name         = "NitroSteggit"
  s.version      = package["version"]
  s.summary      = package["description"]
  s.homepage     = package["homepage"]
  s.license      = package["license"]
  s.authors      = package["author"]

  s.platforms    = { :ios => min_ios_version_supported, :visionos => 1.0 }
  s.source       = { :git => "https://github.com/mrousavy/nitro.git", :tag => "#{s.version}" }

  s.source_files = [
    # Implementation (Swift)
    "ios/**/*.{swift}",
    # Autolinking/Registration (Objective-C++)
    "ios/**/*.{m,mm}",
    # Implementation (C++ objects)
    "cpp/**/*.{hpp,cpp}",
    # Core C implementation - reference directly from core package
    "../core/src/*.c",
    "../core/include/*.h",
  ]

  s.public_header_files = [
    "../core/include/*.h",
  ]

  # Exclude Node.js N-API specific files
  s.exclude_files = [
    "../core/src/wrapper_utils.c",
    "../core/src/wrapper.c",
  ]

  # Add include paths for the core headers
  s.xcconfig = {
    'HEADER_SEARCH_PATHS' => '"$(PODS_TARGET_SRCROOT)/../core/include"'
  }

  load 'nitrogen/generated/ios/NitroSteggit+autolinking.rb'
  add_nitrogen_files(s)

  # System libraries for image processing
  s.libraries = "z", "c++"
  s.frameworks = "ImageIO", "CoreGraphics", "Accelerate"
  
  # Link against libpng and libjpeg
  s.dependency 'libpng'
  s.dependency 'libjpeg-turbo'
  
  s.dependency 'React-jsi'
  s.dependency 'React-callinvoker'
  install_modules_dependencies(s)
end
