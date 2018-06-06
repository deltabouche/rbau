Gem::Specification.new "gemspec", "0.0.1" do |s|
  s.name        = 'rbau'
  s.version     = '0.0.1'
  s.date        = '2018-06-05'
  s.summary     = "Ruby Audio Units Host"
  s.description = "Port of pyau to Ruby (https://github.com/johndpope/pyau)"
  s.authors     = ["Madeline Lim"]
  s.email       = 'deltabouche@gmail.com'
  s.files       = ["lib/rbau.rb", "lib/rbau/rbau.bundle"]
  s.homepage    = 'https://github.com/deltabouche/rbau'
  s.license       = 'MIT'
  s.extensions = %w[ext/rbau/extconf.rb]
end
