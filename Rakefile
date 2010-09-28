require 'rake/clean'
#require 'rake/extensiontask'
require 'rake/gempackagetask'
require 'rake/rdoctask'
require 'lib/gen_eval/version'

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
    s.platform = Gem::Platform::RUBY #'i386-mswin32'
    s.extensions = FileList["ext/**/extconf.rb"]
    s.has_rdoc = true
    s.add_dependency("object2module",">=0.3.0")
    s.files = ["Rakefile", "README.rdoc", "LICENSE", "lib/gen_eval.rb"] + FileList["ext/**/extconf.rb", "ext/**/*.h", "ext/**/*.c", "lib/gen_eval/version.rb"].to_a
end

Rake::GemPackageTask.new(spec) do |pkg|
    pkg.need_zip = false
    pkg.need_tar = false
end

ext_direc = "#{direc}/ext/gen_eval"

file "#{ext_direc}/Makefile" => "#{ext_direc}/extconf.rb" do
  chdir(ext_direc) do
    sh 'ruby extconf.rb'
  end
end

directory "#{direc}/lib/1.9"
directory "#{direc}/lib/1.8"

file "#{ext_direc}/gen_eval.#{dlext}" => ["#{ext_direc}/Makefile", "#{direc}/lib/1.9", "#{direc}/lib/1.8"] do
  chdir(ext_direc) do
    sh 'make'

    if RUBY_VERSION =~ /1.9/
      cp "gen_eval.#{dlext}", "#[direc}/lib/1.9/"
      puts "built gen_eval.#{dlext} and copied to #{direc}/lib/1.9"
    elsif RUBY_VERSION =~ /1.8/
      cp "gen_eval.#{dlext}", "#[direc}/lib/1.8/"
      puts "built gen_eval.#{dlext} and copied to #{direc}/lib/1.8"
    else
      raise "Error, Rakefile only supports Ruby 1.8 and 1.9"
    end
  end
end

task :compile => "#{ext_direc}/gen_eval.#{dlext}"

    
  

# Rake::ExtensionTask.new('mult', spec)  do |ext|
#     ext.config_script = 'extconf.rb' 
#     ext.cross_compile = true                
#     ext.cross_platform = 'i386-mswin32'
# end

Rake::RDocTask.new do |rd|
  rd.main = "README.rdoc"
  rd.rdoc_files.include("README.rdoc", "lib/gen_eval.rb")
end

