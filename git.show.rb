class Git_show
        def initialize()
        end
        class << self
                attr_accessor :commit
                attr_accessor :dry_mode
                attr_accessor :input_f
                attr_accessor :verbose_mode
                def test()
                end
                def go()
                        relative_fn = nil
                        loop do
                                line = Git_show.input_f.gets
                                break unless line
                                begin
                                        line = line.force_encoding('UTF-8')
                                        if line =~ %r{^diff --git a/(.*) b/}
                                                relative_fn=$1
                                        end
                                        if ! relative_fn
                                                if ! Git_show.verbose_mode
                                                        next
                                                end
                                                relative_fn = "RELATIVE_FN__NOT__SET"
                                        end

                                        print "git.show:#{commit}:#{relative_fn}:#{line}"
                                rescue
                                        if Git_show.verbose_mode
                                                puts "exception.............."
                                        end
                                end
                        end
                end
        end
end
j = 0
while ARGV.size > j do
        arg = ARGV[j]
        case arg
        when "-dry"
                Git_show.dry_mode = true
        when /^(-v|-verbose)$/
                Git_show.verbose_mode = true
        when "-commit"
                j = j + 1
                Git_show.commit = ARGV[j]
        else
                input_fn = ARGV[j]
                Git_show.input_f = File.open(input_fn, 'r:UTF-8')
                break
        end
        j += 1
end
Git_show.go
