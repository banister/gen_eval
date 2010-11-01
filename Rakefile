require 'rake/clean'
require 'rake/gempackagetask'
require 'rake/rdoctask'
require './lib/gen_eval/version'

dlext = Config::CONFIG['DLEXT']
direc = File.dirname(__FILE__)

CLEAN.include("ext/**/*.#{dlext}", "ext/**/.log", "ext/**/.o", "ext/**/*~", "ext/**/*#*", "ext/**/.obj", "ext/**/.def", "ext/**/.pdb")
CLOBBER.include("**/*.#{dlext}", "**/*~", "**/*#*", "**/*.log", "**/*.o", "doc/**")

def apply_spec_defaults(s)
    s.name = "gen_eval"
    s.summary = "a strange new breed of instance_eval"
    s.description = s.summary
    s.version = GenEval::VERSION
    s.author = "John Mair (banisterfiend)"
    s.email = 'jrmair@gmail.com'
    s.date = Time.now.strftime '%Y-%m-%d'
    s.require_path = 'lib'
    s.homepage = "http://banisterfiend.wordpress.com"
    s.has_rdoc = 'yard'
    s.add_dependency("object2module",">=0.3.0")
  s.files = FileList["Rakefile", "README.rdoc", "LICENSE", "lib/**/*.rb", "ext/**/extconf.rb", "ext/**/*.h", "ext/**/*.c"].to_a
end

task :test do
  sh "bacon -k #{direc}/test/test.rb"
end

[:mingw32, :mswin32].each do |v|
  namespace v do
    spec = Gem::Specification.new do |s|
      apply_spec_defaults(s)        
      s.platform = "i386-#{v}"
      s.files += FileList["lib/**/*.#{dlext}"].to_a
    end

    Rake::GemPackageTask.new(spec) do |pkg|
      pkg.need_zip = false
      pkg.need_tar = false
    end
  end
end

namespace :ruby do
  spec = Gem::Specification.new do |s|
    apply_spec_defaults(s)        
    s.platform = Gem::Platform::RUBY
    s.extensions = ["ext/gen_eval/extconf.rb"]
  end

  Rake::GemPackageTask.new(spec) do |pkg|
    pkg.need_zip = false
    pkg.need_tar = false
  end
end

