pipeline {
    agent { label 'slave' }
    stages {
        stage('clone code') {
            steps {
                git url: 'https://github.com/shadow1163/dockerfiles'
            }
        }
        stage('build') {
            steps {
                sh 'docker build jenkins -f jenkins/officalDockerfile -t jenkins:github'
            }
        }
        stage('test') {
            agent { docker 'jenkins:github' }
            steps {
                echo 'successfully'
            }
        }
    }
}
