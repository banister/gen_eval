require 'rake/clean'
#require 'rake/extensiontask'
require 'rake/gempackagetask'
require 'rake/rdoctask'
require './lib/gen_eval/version'

dlext = Config::CONFIG['DLEXT']
direc = File.dirname(__FILE__)

CLEAN.include("ext/**/*.#{dlext}", "ext/**/.log", "ext/**/.o", "ext/**/*~", "ext/**/*#*", "ext/**/.obj", "ext/**/.def", "ext/**/.pdb")
CLOBBER.include("**/*.#{dlext}", "**/*~", "**/*#*", "**/*.log", "**/*.o", "doc/**")

spec = Gem::Specification.new do |s|
    s.name = "gen_eval"
    s.summary = "a strange new breed of instance_eval"
    s.description = s.summary
    s.version = GenEval::VERSION
    s.author = "John Mair (banisterfiend)"
    s.email = 'jrmair@gmail.com'
    s.date = Time.now.strftime '%Y-%m-%d'
    s.require_path = 'lib'
    s.homepage = "http://banisterfiend.wordpress.com"
#  s.platform = 'i386-mswin32'
    s.platform = Gem::Platform::RUBY

    s.extensions = FileList["ext/**/extconf.rb"]
    s.has_rdoc = true
    s.add_dependency("object2module",">=0.3.0")
  s.files = ["Rakefile", "README.rdoc", "LICENSE", "lib/gen_eval.rb"] + FileList["ext/**/extconf.rb", "ext/**/*.h", "ext/**/*.c", "lib/gen_eval/version.rb"]# +
  #  ["lib/1.8/gen_eval.so", "lib/1.9/gen_eval.so"]
end

Rake::GemPackageTask.new(spec) do |pkg|
    pkg.need_zip = false
    pkg.need_tar = false
end

Rake::RDocTask.new do |rd|
  rd.main = "README.rdoc"
  rd.rdoc_files.include("README.rdoc", "lib/gen_eval.rb")
end

